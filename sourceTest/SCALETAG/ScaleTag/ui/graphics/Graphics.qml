import QtQuick 2.0
import VTK 9.3

Item {
    id: renderItem
    anchors.fill: parent

    VTKRenderWindow{
        id:vtkwindow
        anchors.fill:parent
        //color:"#efefef"
    }
    VTKRenderItem{
        objectName:"visual"
        x:0
        y:0
        anchors.fill:parent
       // color:"#efefef"

        renderWindow:vtkwindow
    }

}
