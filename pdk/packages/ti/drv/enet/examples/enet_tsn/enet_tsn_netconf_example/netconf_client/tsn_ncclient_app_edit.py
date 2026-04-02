#/*
# * Copyright (c) 2023 Texas Instruments Incorporated
# * Copyright (c) 2023 Excelfore Corporation (https://excelfore.com)
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
Netconf Client Application GUI using PySide (Edit Config Window)
"""
from PySide6.QtWidgets import (
    QMainWindow,
    QVBoxLayout,
    QHBoxLayout,
    QWidget,
    QPushButton,
    QTextEdit,
    QFileDialog,
    QMessageBox
)
from PySide6.QtCore import Signal
from lxml import etree

class EditConfigWindow(QMainWindow):
    edit_config_result = Signal(bool, etree._Element)

    class InvalidRootElementException(Exception):
        pass

    def __init__(self, parent=None):
        super().__init__(parent)
        self.main_window = parent

        self.setWindowTitle("Edit Config")
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.load_button = QPushButton("Load XML from File")
        # Layout for the buttons at the top
        button_layout = QHBoxLayout()
        button_layout.addWidget(self.load_button)
        button_layout.addStretch(1)

        layout = QVBoxLayout()
        self.central_widget.setLayout(layout)

        # Add the button layout
        layout.addLayout(button_layout)

        # Text edit area for XML input
        self.xml_input = QTextEdit()
        layout.addWidget(self.xml_input)

        # Send button
        send_button = QPushButton("Send")
        send_button.clicked.connect(self.send_edit_config_rpc)
        layout.addWidget(send_button)

        self.edit_config_result.connect(self.edit_config_result_cb)
        self.load_button.clicked.connect(self.load_xml_from_file)
        self.setFixedSize(640, 480)

    def send_edit_config_rpc(self):
        sample_config = '''
<data xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
  <interfaces xmlns="urn:ietf:params:xml:ns:yang:ietf-interfaces">
    <!-- # config parameters to edit -->
  </interfaces>
</data>
'''
        xml_input_str = self.xml_input.toPlainText()
        try:
            encoded_xml_input_str = xml_input_str.encode('UTF-8')
            xml_data = etree.fromstring(encoded_xml_input_str)
            if self.main_window:
                self.validate_input_data(xml_data)
                self.main_window.edit_config_rpc.emit(xml_data)
        except self.InvalidRootElementException as e:
            QMessageBox.warning(self, "Root Element Error",
                    f"{e}:\nSample Config Format:{sample_config}")
        except etree.XMLSyntaxError as e:
            QMessageBox.warning(self, "XML Error", f"Invalid XML: {str(e)}")

    def validate_input_data(self, data):
        # parse xml data
        root_element=etree.QName(data).localname
        if root_element != 'data':
            error_message = f"Invalid root element: <{root_element}>"
            raise self.InvalidRootElementException(error_message)

    def edit_config_result_cb(self, success, rpc_error):
        if success:
            QMessageBox.information(self, "Success", "Edit Config Operation Success")
            self.close()
        else:
            etree.indent(rpc_error)
            rpc_error_str = etree.tostring(rpc_error, pretty_print=True,
                    encoding="UTF-8", xml_declaration=True)
            error_message = f"Reply Error:\n{rpc_error_str.decode()}"
            QMessageBox.warning(self, "RPC Operation Failed", error_message)

    def load_xml_from_file(self):
        file_dialog = QFileDialog(self)
        file_dialog.setFileMode(QFileDialog.FileMode.ExistingFile)
        file_dialog.setNameFilter("XML files (*.xml)")
        if file_dialog.exec_():
            selected_files = file_dialog.selectedFiles()
            if selected_files:
                file_path = selected_files[0]
                with open(file_path, 'r') as file:
                    xml_data = file.read()
                    self.xml_input.setPlainText(xml_data)

    def closeEvent(self, event):
        if self.main_window:
            self.main_window.setEnabled(True)
        super().closeEvent(event)

