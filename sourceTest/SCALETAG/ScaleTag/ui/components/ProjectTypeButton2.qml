import QtQuick 2.7
import assets 1.0

Item{
    //property alias  buttonIcon: projectTypeButtonIcon.text
    property alias  buttonIcon: projectTypeButtonIcon.source
    property alias  buttonLabel: projectTypeButtonLabel.text

    signal projectTypeButtonClicked()


id: projectTypeButton
width: Style.projectTypeButtonWidth
height: Style.projectTypeButtonHeight

Rectangle{
    id:projectTypeButtonContainer
    anchors.fill: parent
    color:"#cccccc"


Row{
anchors.fill:parent
Image{id: projectTypeButtonIcon
    source: "qrc:/logos/abaqus"
    height: Style.projectTypeButtonIconWidth
    width: Style.projectTypeButtonIconHeight
}



Text {
    id: projectTypeButtonLabel
    text: " "
    width: Style.projectTypeButtonLabelWidth
    height: Style.projectTypeButtonLabelHeight

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    color: "#000000"
}
/*Image{
    id:projectTypeButtonIcon
    height: Style.projectTypeButtonIconWidth
    width: Style.projectTypeButtonIconHeight
    source:"qrc:/logos/marc"
    //horizontalAlignment: Text.AlignHCenter
    //verticalAlignment: Text.AlignVCenter

   // color: "#ffffff"
}*/

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
           color: Style.hoverColour
        }}]




    }


}
