import QtQuick 2.7
import QtQuick.Controls 2.7
import assets 1.0

Item{
id:simulationButton
property alias buttonIcon: simulationButtonIcon.text
property alias tooltip: buttonLabel.toolTipText
property color buttonColor: "#000000"
//property color buttonHoveredColor: Qt.lighter(buttonColor)
signal simulationButtonClicked()

width: Style.simulationButtonWidth
height: Style.simulationButtonHeight

Rectangle{
id: simulationButtonContainer
anchors.fill: parent

Text{
id: simulationButtonIcon
color:buttonColor
font{
    family:Style.fontawesome
    pixelSize:50
}
text:""
anchors.fill:parent
horizontalAlignment: Text.AlignHCenter
verticalAlignment: Text.AlignVCenter
MouseArea{
id:buttonMouseArea
anchors.fill:parent
hoverEnabled: true
onEntered: simulationButtonContainer.state="hover";

onExited: simulationButtonContainer.state=""
onClicked: simulationButton.simulationButtonClicked()}


}
states: [
    State{
        name: "hover"
        PropertyChanges{
           target: simulationButtonIcon
           //color: Style.hoverColour
           color:Qt.lighter(buttonColor)
        }}
    ]
}


Label {
    id: buttonLabel
        text: ""
        property string toolTipText: "set me"
        ToolTip.text: toolTipText
        ToolTip.visible: toolTipText ? buttonMouseArea.containsMouse : false

    }

}
