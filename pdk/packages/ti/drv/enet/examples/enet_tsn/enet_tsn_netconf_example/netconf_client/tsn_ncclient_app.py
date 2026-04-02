#!/usr/bin/python3
#/*
# * Copyright (c) 2024 Texas Instruments Incorporated
# * Copyright (c) 2024 Excelfore Corporation (https://excelfore.com)
# *
# * All rights reserved not granted herein.
# * Limited License.
# *
# * Texas Instruments Incorporated grants a world-wide, royalty-free,
# * non-exclusive license under copyrights and patents it now or hereafter
# * owns or controls to make, have made, use, import, offer to sell and sell ("Utilize")
# * this software subject to the terms herein. With respect to the foregoing patent
# * license, such license is granted solely to the extent that any such patent is necessary
# * to Utilize the software alone. The patent license shall not apply to any combinations which
# * include this software, other than combinations with devices manufactured by or for TI ("TI Devices").
# * No hardware patent is licensed hereunder.
# *
# * Redistributions must preserve existing copyright notices and reproduce this license (including the
# * above copyright notice and the disclaimer and (if applicable) source code license limitations below)
# * in the documentation and/or other materials provided with the distribution
# *
# * Redistribution and use in binary form, without modification, are permitted provided that the following
# * conditions are met:
# *
# * * No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any
# * software provided in binary form.
# * * any redistribution and use are licensed by TI for use only with TI Devices.
# * * Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
# *
# * If software source code is provided to you, modification and redistribution of the source code are permitted
# * provided that the following conditions are met:
# *
# * * any redistribution and use of the source code, including any resulting derivative works, are licensed by
# * TI for use only with TI Devices.
# * * any redistribution and use of any object code compiled from the source code and any resulting derivative
# * works, are licensed by TI for use only with TI Devices.
# *
# * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or
# * promote products derived from this software without specific prior written permission.
# *
# * DISCLAIMER.
# *
# * THIS SOFTWARE IS PROVIDED BY TI AND TI"S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# * IN NO EVENT SHALL TI AND TI"S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# * POSSIBILITY OF SUCH DAMAGE.
#*/
"""
Netconf Client Application GUI using PySide (Main module)
"""
from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QScreen
from tsn_ncclient_app_login import NconfLoginWindow
from tsn_ncclient_app_data import NconfDataWindow
from tsn_ncclient import TsnNetconfClient

ncclient = TsnNetconfClient()
login_window = None
main_window = None

def connection_result_handler(success):
    if success:
        if login_window:
            login_window.connection_result.emit(success)
        if main_window:
            # get initial config and status data to populate main window tree
            ncclient.get_rpc_proc()
            ncclient.get_config_rpc_proc()
            main_window.show()
    else:
        print("Connection to NETCONF server failed!")

def config_data_update_handler(success, config_data_elements):
    if main_window:
        main_window.config_data_update.emit(success, config_data_elements)

def status_data_update_handler(success, status_data_elements):
    if main_window:
        main_window.status_data_update.emit(success, status_data_elements)

def edit_config_result_handler(success, rpc_error):
    if main_window:
        reply_ele = rpc_error.reply_ele if rpc_error is not None else None
        main_window.edit_config_result.emit(success, reply_ele)

if __name__ == "__main__":
    app = QApplication([])
    screen_geometry = QScreen.availableGeometry(QApplication.primaryScreen())
    login_window = NconfLoginWindow(screen_geometry)
    login_window.connect_to_server.connect(ncclient.connect_to_server)
    main_window = NconfDataWindow(
        ncclient.get_rpc_proc,
        ncclient.get_config_rpc_proc,
        ncclient.edit_config_rpc_proc,
        ncclient.close_session_rpc_proc
    )
    result_callbacks = (connection_result_handler,
            edit_config_result_handler,
            config_data_update_handler,
            status_data_update_handler)
    ncclient.set_result_callbacks(result_callbacks)
    login_window.show()
    app.exec()

