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
Netconf Client Application GUI using PySide (Main/Data Window)
"""
from PySide6.QtGui import QStandardItem, QStandardItemModel, QBrush, QColor
from PySide6.QtWidgets import (
    QMainWindow,
    QPushButton,
    QVBoxLayout,
    QWidget,
    QTreeView,
    QTabWidget
)
from PySide6.QtCore import Signal
from tsn_ncclient_app_edit import EditConfigWindow
from lxml import etree

class NconfDataWindow(QMainWindow):
    get_rpc = Signal(str)
    get_config_rpc = Signal(str)
    edit_config_rpc = Signal(etree._Element)
    close_session_rpc = Signal()
    config_data_update = Signal(bool, etree._Element)
    status_data_update = Signal(bool, etree._Element)
    edit_config_result = Signal(bool, etree._Element)
    def __init__(self, get_cb, get_config_cb, edit_config_cb, close_cb):
        super().__init__()
        self.setWindowTitle("Netconf Client")

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)

        self.data_layout = QVBoxLayout()
        self.central_widget.setLayout(self.data_layout)

        # Create tab widget
        self.tab_widget = QTabWidget()
        self.data_layout.addWidget(self.tab_widget)

        # Create and add "Config" tab
        self.config_tab = QWidget()
        self.tab_widget.addTab(self.config_tab, "Config")
        self.config_layout = QVBoxLayout(self.config_tab)

        # Create tree view for displaying XML data in the "Config" tab
        self.config_tree_view = QTreeView()
        self.config_layout.addWidget(self.config_tree_view)

        # Create a model for the "Config" tab tree view
        self.config_model = QStandardItemModel()
        self.config_model.setColumnCount(2)
        self.config_model.setHorizontalHeaderLabels(["", ""])
        self.config_tree_view.setModel(self.config_model)

        self.config_refresh_button = QPushButton("Edit Config")
        self.config_refresh_button.clicked.connect(self.edit_config_data)
        self.config_layout.addWidget(self.config_refresh_button)

        # Create and add "Status" tab
        self.status_tab = QWidget()
        self.tab_widget.addTab(self.status_tab, "Status")
        self.status_layout = QVBoxLayout(self.status_tab)

        # Create tree view for displaying XML data in the "Status" tab
        self.status_tree_view = QTreeView()
        self.status_layout.addWidget(self.status_tree_view)

        # Create a model for the "Status" tab tree view
        self.status_model = QStandardItemModel()
        self.status_model.setColumnCount(2)
        self.status_model.setHorizontalHeaderLabels(["", ""])
        self.status_tree_view.setModel(self.status_model)
        self.status_tree_view.setColumnWidth(0, 400)

        self.status_refresh_button = QPushButton("Refresh")
        self.status_refresh_button.clicked.connect(self.refresh_status_data)
        self.status_layout.addWidget(self.status_refresh_button)

        if get_cb:
            self.get_rpc.connect(get_cb)

        if get_config_cb:
            self.get_config_rpc.connect(get_config_cb)

        if edit_config_cb:
            self.edit_config_rpc.connect(edit_config_cb)

        if close_cb:
            self.close_session_rpc.connect(close_cb)

        self.config_data_update.connect(self.update_config_data)
        self.status_data_update.connect(self.update_status_data)
        self.edit_config_result.connect(self.emit_edit_result)
        self.edit_window = None

        self.setFixedSize(800, 600)

    def update_config_data(self, success, config_data_elements):
        if success:
            self.populate_tree_view(self.config_tree_view, self.config_model,
                    config_data_elements)
            self.config_tree_view.setColumnWidth(0, 400)

    def update_status_data(self, success, status_data_elements):
        if success:
            self.populate_tree_view(self.status_tree_view,
                    self.status_model, status_data_elements)
            self.status_tree_view.setColumnWidth(0, 400)

    def emit_edit_result(self, success, rpc_error):
        if self.edit_window:
            self.edit_window.edit_config_result.emit(success, rpc_error)
            if success:
                self.refresh_config_data()

    def edit_config_data(self):
        self.setEnabled(False)
        self.edit_window = EditConfigWindow(self)
        self.edit_window.show()

    def refresh_config_data(self):
        self.get_config_rpc.emit(None)

    def refresh_status_data(self):
        self.get_rpc.emit(None)

    def populate_tree_view(self, tree_view, model, xml_data):
        model.clear()
        root_item = model.invisibleRootItem()

        try:
            self.add_xml_to_model(tree_view, model, xml_data, root_item)
        except etree.XMLSyntaxError as e:
            print("XML parsing error:", e)

    def add_xml_to_model(self, tree_view, model, xml_element, parent_item):
        node = QStandardItem(etree.QName(xml_element).localname)
        if xml_element.text and xml_element.text.strip():
            value = QStandardItem(xml_element.text.strip())
            value.setForeground(QBrush(QColor(0, 150, 20)))
            parent_item.appendRow([node, value])
        else:
            parent_item.appendRow(node)

        tree_view.setExpanded(node.index(), True)
        for child in xml_element:
            self.add_xml_to_model(tree_view, model, child, node)

        model.setHorizontalHeaderLabels(["", ""])

    def closeEvent(self, event):
        self.close_session_rpc.emit()
        super().closeEvent(event)

