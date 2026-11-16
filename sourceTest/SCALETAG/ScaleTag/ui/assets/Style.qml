pragma Singleton
import QtQuick 2.7


Item{

//Some fonts
FontLoader{
id:fontawesomeLoader
source:"qrc:/assets/fontawesome"
}

property alias fontawesome:fontawesomeLoader.name

//For Project type buttons (Abaqus, ScaleTag, MSC marc etc.
readonly property real projectTypeButtonWidth: /*projectTypeButtonIconWidth +*/ projectTypeButtonLabelWidth
readonly property real projectTypeButtonHeight: Math.max(projectTypeButtonIconHeight, projectTypeButtonLabelHeight)
//readonly property real projectTypeButtonIconWidth: 80
property real projectTypeButtonIconWidth: 65
 property real projectTypeButtonIconHeight: projectTypeButtonIconWidth
//readonly property real projectTypeButtonLabelWidth: 160
property real projectTypeButtonLabelWidth: 150
property real projectTypeButtonLabelHeight: projectTypeButtonIconWidth



readonly property color projectTypeButtonsBarColour:"#000000"
readonly property color hoverColour: "#888888"
readonly property color projectTypeHomeViewColor: "#efefef"


//Simulation button style
readonly property real simulationButtonsBarWidth: 60
readonly property real simulationButtonWidth: simulationButtonsBarWidth
readonly property real simulationButtonHeight: simulationButtonsBarWidth

// Welcome View
readonly property color welcomeViewColour: "#006600"
readonly property color welcomeViewColourHovered: Qt.darker(welcomeViewColour)

}

