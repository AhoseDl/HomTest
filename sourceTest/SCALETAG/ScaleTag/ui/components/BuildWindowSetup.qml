import QtQuick 2.0
//import QtQuick.Window 2.3
import QtQuick.Controls 2.5

Item {
    id:windowSetup
    anchors.fill: parent
    property alias windowMenuBar: parent.menubar
    property alias windowHeader: parent.header
    property alias windowFooter: parent.footer

    property ToolsProvider toolsProvider

    //window
    //Setup bindings between properties of BuildWindowSetup && ApplicationWindow (parent)
    //Create a class ToolProvider To feed the windowSetup with the right properties

    MenuBar{
        Repeater{
            id:menuRepeater

            delegate: Menu{
            tile:{}
            enabled:true
            Action{
            text:qsTr("")
            onTriggered: {}
            }
            }
        }
    }

}
