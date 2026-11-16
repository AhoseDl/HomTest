import QtQuick 2.7
import QtQuick.Window 2.7
import assets 1.0

Item{
id:abaqusHomeView
clip:true
anchors.fill:parent
Rectangle{
    id:abaqusHomeFrame
    anchors.fill:parent
    color: Style.projectTypeHomeViewColor

 Text{
     anchors.centerIn: parent
text: "Abaqus Home View"
 }
}
}
