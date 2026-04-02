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
Netconf Client RPC Handler using netconf_client PyPI module
"""
import socket
from lxml import etree
from netconf_client.connect import connect_ssh
from netconf_client.connect import connect_tls
from netconf_client.session import Session
from netconf_client.ncclient import Manager
from netconf_client.error import RpcError

class TsnNetconfClient():
    def __init__(self):
        self.nconf_session = None
        self.nconf_sessmngr = None
        self.connection_result_cb = None
        self.edit_config_result_cb = None
        self.get_config_result_cb = None
        self.get_result_cb = None

    def set_result_callbacks(self, result_callbacks):
        """Set RPC operations callbacks
        :param tuple result_callbacks: set of callback methods for the following
                                       operation:
                                       - connect_to_server
                                       - <get>
                                       - <get-config>
                                       - <edit-config>
                                       - <close-session>
        """
        self.connection_result_cb,  \
        self.edit_config_result_cb, \
        self.get_config_result_cb, \
        self.get_result_cb = result_callbacks

    def connect_to_server(self, transport_type, connection_details):
        """Connect to a NETCONF server
        :param str transport_type: either 'SSH', 'TLS', 'TCP'
        :param tuple connection_details: connection information needed depending
                                         on transport type
        """
        success = False
        if transport_type == "SSH":
            server_host, user_name, user_password = connection_details
            self.nconf_session = connect_ssh(host=server_host,
                    username=user_name, password=user_password)
            self.nconf_sessmngr = Manager(self.nconf_session, timeout=120)
            success = True
        elif transport_type == "TLS":
            server_host, user_name, clcert, clkey, cacert = connection_details
            self.nconf_session = connect_tls(host=server_host,
                    keyfile=clkey, certfile=clcert, ca_certs=cacert)
            self.nconf_sessmngr = Manager(self.nconf_session, timeout=120)
            success = True
        elif transport_type == "TCP":
            server_host, user_name = connection_details
            self.nconf_session = self.connect_tcp(host=server_host, username=user_name)
            self.nconf_sessmngr = Manager(self.nconf_session, timeout=120)
            success = True
        else:
            print(f"Unknown transport type: {transport_type}!!!")

        if self.connection_result_cb:
            self.connection_result_cb(success)

    def get_rpc_proc(self, _filter=None):
        """Perform a <get> rpc request
        :param str _filter: The ``<filter>`` node to use in the request
        """
        if self.nconf_sessmngr:
            get_reply = self.nconf_sessmngr.get(filter=_filter)
            if self.get_result_cb:
                self.get_result_cb(True, get_reply.data_ele)
        else:
            print("Client not yet connected to any Server")

    def get_config_rpc_proc(self, _filter=None):
        """Perform a <get-config> rpc request
        :param str _filter: The ``<filter>`` node to use in the request
        """
        if self.nconf_sessmngr:
            get_config_reply = self.nconf_sessmngr.get_config(filter=_filter)
            if self.get_config_result_cb:
                self.get_config_result_cb(True, get_config_reply.data_ele)
        else:
            print("Client not yet connected to any Server")

    def edit_config_rpc_proc(self, xml_data):
        """Perform an <edit-config> rpc request
        :param lxml.Element xml_data: yang config to be edited in lxml etree
                                      element format
        """
        if self.nconf_sessmngr:
            config = self._construct_config_data(xml_data)
            config_str = etree.tostring(config, encoding="UTF-8")
            try:
                self.nconf_sessmngr.edit_config(config=config_str.decode())
                if self.edit_config_result_cb:
                    self.edit_config_result_cb(True, None)
            except RpcError as e:
                if self.edit_config_result_cb:
                    self.edit_config_result_cb(False, e)
        else:
            print("Client not yet connected to any Server")

    def close_session_rpc_proc(self):
        """Perform a <close-session> rpc request
        """
        if self.nconf_sessmngr:
            self.nconf_sessmngr.close_session()
        else:
            print("Client not yet connected to any Server")

    def connect_tcp(self,
        host=None,
        port=10830,
        username="netconf",
        sock=None,
        timeout=120,
        initial_timeout=None,
        general_timeout=None,
    ):
        """Connect to a NETCONF server over Plaintext TCP.
        This is a custom implmentation using netconf_client PyPi's existing
        classes and methods

        :param str host: Hostname or IP address; unused if an already-open
                         socket is provided

        :param int port: TCP port to initiate the connection; unused if an
                         already-open socket is provided

        :param str username: Username to login with; always required

        :param sock: An already-open TCP socket; SSH will be setup on top
                     of it

        :param int initial_timeout: Seconds to wait when first connecting the socket.

        :param int general_timeout: Seconds to wait for a response from the server after connecting.

        :param int timeout: (Deprecated) Seconds to wait when connecting the socket if initial_timeout is None.  This will
                            be ignored if initial_timeout is not None, and will be removed in the next major release.
        """
        if not sock:
            sock = socket.socket()
            sock.settimeout(initial_timeout or timeout)
            sock.connect((host, port))
            sock.settimeout(general_timeout)
        src_ip_addr = sock.getsockname()[0]
        src_port = sock.getsockname()[1]
        connection_details = f"{src_ip_addr} {src_port} {host} {port} {username}"
        if host.lower() in ('localhost', '127.0.0.1'):
            sock.send(connection_details.encode())
            response = sock.recv(1024)
            if response.decode() != "WDROQ09ORjogU1NIX0NPTk5FQ1RJT04gT0s=":
                sock.close()
        print(f"Connection Details: {connection_details}")
        return Session(sock)

    def _construct_config_data(self, xml_data):
        config = etree.Element('config')
        for child in xml_data:
            config.append(child)
        return config

