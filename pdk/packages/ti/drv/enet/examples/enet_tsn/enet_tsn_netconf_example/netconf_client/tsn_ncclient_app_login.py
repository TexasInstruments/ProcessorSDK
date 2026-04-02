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
Netconf Client Application GUI using PySide (Login Window)
"""
import os
import configparser
from PySide6.QtWidgets import (
    QPushButton,
    QVBoxLayout,
    QHBoxLayout,
    QDialog,
    QLabel,
    QLineEdit,
    QRadioButton,
    QFileDialog
)
from PySide6.QtCore import Signal

class NconfLoginWindow(QDialog):
    connect_to_server = Signal(str, tuple)
    connection_result = Signal(bool)
    def __init__(self, screen_geometry):
        super().__init__()
        self.cache_file = os.path.join(os.path.dirname(__file__), '.cache.ini')
        self.config_cache_init()

        self.setWindowTitle("NETCONF Server Information")
        self.login_layout = QVBoxLayout()
        self.input_layouts = {}

        self.server_host_input=self.add_input_layout("NETCONF Server Host", None)
        self.login_layout.addLayout(self.server_host_input[0])
        self.username_input=self.add_input_layout("Username", None)
        self.login_layout.addLayout(self.username_input[0])

        self.ssh_password_input=self.add_input_layout("Password", None)
        self.ssh_password_input[3].setEchoMode(QLineEdit.EchoMode.Password)
        self.login_layout.addLayout(self.ssh_password_input[0])

        self.tls_cert_input=self.add_input_layout("Client Certificate File", "Browse")
        self.login_layout.addLayout(self.tls_cert_input[0])

        self.tls_key_input=self.add_input_layout("Client Certificate Key File", "Browse")
        self.login_layout.addLayout(self.tls_key_input[0])

        self.tls_ca_input=self.add_input_layout("CA Certificate File", "Browse")
        self.login_layout.addLayout(self.tls_ca_input[0])

        self.server_type_label = QLabel("Transport Selection")
        self.login_layout.addWidget(self.server_type_label)

        self.ssh_radio = QRadioButton("SSH")
        self.ssh_radio.toggled.connect(self.load_ssh_transport_inputs)
        self.login_layout.addWidget(self.ssh_radio)
        self.tls_radio = QRadioButton("TLS")
        self.login_layout.addWidget(self.tls_radio)
        self.tls_radio.toggled.connect(self.load_tls_transport_inputs)
        self.tcp_radio = QRadioButton("TCP")
        self.login_layout.addWidget(self.tcp_radio)
        self.tcp_radio.toggled.connect(self.load_tcp_transport_inputs)

        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(self.connect_proc)
        self.login_layout.addWidget(self.connect_button)
        self.setLayout(self.login_layout)

        #Set X Position Center
        self.x_pos = (screen_geometry.width() - self.width())/2
        #Set Y Position Center
        self.y_pos = (screen_geometry.height() - self.height())/2

        # Set Default Layout
        self.load_inputs_from_cache()

        # Set Signal callback
        self.connection_result.connect(self.connect_callback)

    def config_cache_init(self):
        self.cache_inputs = configparser.ConfigParser()
        if os.path.isfile(self.cache_file):
            self.cache_inputs.read(self.cache_file)

    def load_inputs_from_cache(self):
        if ('COMMON' in self.cache_inputs and
            'transport' in self.cache_inputs['COMMON']):
            if self.cache_inputs['COMMON']['transport'] == 'SSH':
                self.ssh_radio.setChecked(True)
                self.load_ssh_transport_inputs()
            elif self.cache_inputs['COMMON']['transport'] == 'TLS':
                self.tls_radio.setChecked(True)
                self.load_tls_transport_inputs()
            else:
                self.tcp_radio.setChecked(True)
                self.load_tcp_transport_inputs()
        else:
            self.cache_inputs['COMMON'] = { 'transport': 'TCP' }
            self.tcp_radio.setChecked(True)
            self.load_tcp_transport_inputs()

    def load_common_inputs(self, transport_inputs):
        if self.server_host_input[2].text() in transport_inputs:
            server_host = transport_inputs[self.server_host_input[2].text()]
            self.server_host_input[3].setText(server_host)
        if self.username_input[2].text() in transport_inputs:
            user_name = transport_inputs[self.username_input[2].text()]
            self.username_input[3].setText(user_name)

    def load_ssh_transport_inputs(self):
        if 'SSH' in self.cache_inputs:
            ssh_inputs = self.cache_inputs['SSH']
            self.load_common_inputs(ssh_inputs)
            if self.ssh_password_input[2].text() in ssh_inputs:
                ssh_password = ssh_inputs[self.ssh_password_input[2].text()]
                self.ssh_password_input[3].setText(ssh_password)
        else:
            self.server_host_input[3].setText("")
            self.username_input[3].setText("")
        self.load_ssh_inputs()

    def load_tls_transport_inputs(self):
        if 'TLS' in self.cache_inputs:
            tls_inputs = self.cache_inputs['TLS']
            self.load_common_inputs(tls_inputs)
            if self.tls_cert_input[2].text() in tls_inputs:
                client_cert = tls_inputs[self.tls_cert_input[2].text()]
                self.tls_cert_input[3].setText(client_cert)
            if self.tls_key_input[2].text() in tls_inputs:
                client_key = tls_inputs[self.tls_key_input[2].text()]
                self.tls_key_input[3].setText(client_key)
            if self.tls_ca_input[2].text() in tls_inputs:
                ca_cert = tls_inputs[self.tls_ca_input[2].text()]
                self.tls_ca_input[3].setText(ca_cert)
        else:
            self.server_host_input[3].setText("")
            self.username_input[3].setText("")
        self.load_tls_inputs()

    def load_tcp_transport_inputs(self):
        if 'TCP' in self.cache_inputs:
            tcp_inputs = self.cache_inputs['TCP']
            self.load_common_inputs(tcp_inputs)
        else:
            self.server_host_input[3].setText("")
            self.username_input[3].setText("")
        self.load_tcp_inputs()

    def save_inputs_to_cache(self):
        with open(self.cache_file, 'w') as configfile:
            self.cache_inputs.write(configfile)

    def add_input_layout(self, label_text, button_text=None):
        vlayout = QVBoxLayout()
        hlayout = QHBoxLayout()
        input_label = QLabel(label_text)
        input_editbox = QLineEdit()
        input_button = None
        vlayout.addWidget(input_label)
        hlayout.addWidget(input_editbox)
        if button_text:
            input_button = QPushButton(button_text)
            input_button.clicked.connect(lambda: self.browse_file(input_editbox))
            hlayout.addWidget(input_button)
        vlayout.addLayout(hlayout)
        self.input_layouts[label_text] = (vlayout, hlayout, input_label,
                input_editbox, input_button)
        return self.input_layouts[label_text]

    def browse_file(self, line_edit):
        file_dialog = QFileDialog(self)
        file_path, _ = file_dialog.getOpenFileName(self, "Open File", "", "All Files (*)")
        if file_path:
            line_edit.setText(file_path)

    def load_ssh_inputs(self):
        if self.ssh_radio.isChecked():
            self.hide_tls_inputs()
            self.hide_tcp_inputs()
            self.show_input_widgets(self.ssh_password_input)
            self.setFixedSize(450, 286)
            if self.x_pos and self.y_pos:
                self.move(self.x_pos, self.y_pos)

    def load_tls_inputs(self):
        if self.tls_radio.isChecked():
            self.hide_ssh_inputs()
            self.hide_tcp_inputs()
            self.show_input_widgets(self.tls_cert_input)
            self.show_input_widgets(self.tls_key_input)
            self.show_input_widgets(self.tls_ca_input)
            self.setFixedSize(450, 382)
            if self.x_pos and self.y_pos:
                self.move(self.x_pos, self.y_pos)

    def load_tcp_inputs(self):
        if self.tcp_radio.isChecked():
            self.hide_ssh_inputs()
            self.hide_tls_inputs()
            self.setFixedSize(450, 238)
            if self.x_pos and self.y_pos:
                self.move(self.x_pos, self.y_pos)

    def hide_ssh_inputs(self):
        self.hide_input_widgets(self.ssh_password_input)

    def hide_tls_inputs(self):
        self.hide_input_widgets(self.tls_cert_input)
        self.hide_input_widgets(self.tls_key_input)
        self.hide_input_widgets(self.tls_ca_input)

    def hide_tcp_inputs(self):
        # Do Nothing
        pass

    def show_input_widgets(self, input_dict):
        input_dict[2].show()
        input_dict[3].show()
        if input_dict[4]:
            input_dict[4].show()

    def hide_input_widgets(self, input_dict):
        input_dict[2].hide()
        input_dict[3].hide()
        if input_dict[4]:
            input_dict[4].hide()

    def set_fixed_position(self, x, y):
        self.x_pos = x
        self.y_pos = y

    def connect_proc(self):
        transport_type=None
        connection_details = None
        transport_specific_input = None
        if self.ssh_radio.isChecked():
            transport_type = "SSH"
            transport_specific_input = {
                self.ssh_password_input[2].text(): self.ssh_password_input[3].text()
            }
            connection_details = (
                self.server_host_input[3].text(),
                self.username_input[3].text(),
                self.ssh_password_input[3].text()
            )
        elif self.tls_radio.isChecked():
            transport_type = "TLS"
            transport_specific_input = {
                self.tls_cert_input[2].text(): self.tls_cert_input[3].text(),
                self.tls_key_input[2].text(): self.tls_key_input[3].text(),
                self.tls_ca_input[2].text(): self.tls_ca_input[3].text()
            }
            connection_details = (
                self.server_host_input[3].text(),
                self.username_input[3].text(),
                self.tls_cert_input[3].text(),
                self.tls_key_input[3].text(),
                self.tls_ca_input[3].text()
            )
        else:
            transport_type = "TCP"
            connection_details = (
                self.server_host_input[3].text(),
                self.username_input[3].text()
            )
        self.cache_inputs['COMMON'] = { 'transport': transport_type }
        self.cache_inputs[transport_type] = {
            self.server_host_input[2].text(): self.server_host_input[3].text(),
            self.username_input[2].text(): self.username_input[3].text()
        }
        if transport_specific_input:
            self.cache_inputs[transport_type].update(transport_specific_input)
        self.connect_to_server.emit(transport_type, connection_details)

    def connect_callback(self, success):
        if success:
            print("Connection successful")
            self.save_inputs_to_cache()
            self.close()
        else:
            print("Connection failed")
