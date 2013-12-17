# _wmi_object: \\BLUEDON-86\root\MicrosoftIISv2:IIsWebServerSetting.Name="W3SVC/1" .Port

import wmi
import sys

def changeport(newport):
    webservers = wmi.WMI(namespace="MicrosoftIISv2").IIsWebServerSetting()
    for webserver in webservers:
        # ip, hostname, port
        webserver.ServerBindings[0].port = newport
        webserver.put()

if __name__ == '__main__':
    if len(sys.argv) == 2:
        changeport(int(sys.argv[1]))
