import QtQuick 2.7
import assets 1.0

Item{
//    property alias  buttonIcon: projectTypeButtonIcon.text
    property alias  buttonLabel: projectTypeButtonLabel.text

    //
    property alias welcomeViewColour:projectTypeButtonContainer.color
    //property alias welcomeViewColourHovered:Qt.darker(projectTypeButtonContainer.color)

    signal projectTypeButtonClicked()


id: projectTypeButton
width: Style.projectTypeButtonWidth
height: Style.projectTypeButtonHeight

Rectangle{
    id:projectTypeButtonContainer
    anchors.fill: parent
    color:Style.welcomeViewColour//"#00ff00"


Row{
anchors.fill:parent
/*Image{id: projectTypeButtonIcon
    source: "qrc:/logos/abaquslogo"
    height: Style.projectTypeButtonIconWidth
    width: Style.projectTypeButtonIconHeight
}*/

//Text{
//    id:projectTypeButtonIcon
//    height: Style.projectTypeButtonIconWidth
//    width: Style.projectTypeButtonIconHeight
//    font{
//        family:Style.fontawesome
//        pixelSize:42}
//    text:""
//    horizontalAlignment: Text.AlignHCenter
//    verticalAlignment: Text.AlignVCenter

//    color: "#ffffff"
//}

Text {
    id: projectTypeButtonLabel
    text: " "
    width: Style.projectTypeButtonLabelWidth
    height: Style.projectTypeButtonLabelHeight

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    color: "#ffffff"
}

}

MouseArea{
id:buttonMouseArea
anchors.fill:parent
hoverEnabled: true
onEntered: projectTypeButtonContainer.state="hover"
onExited: projectTypeButtonContainer.state=""
onClicked: projectTypeButton.projectTypeButtonClicked()}

states: [
    State{
        name: "hover"
        PropertyChanges{
           target: projectTypeButtonContainer
           color: Style.welcomeViewColourHovered//Style.hoverColour
        }}]




    }


}
