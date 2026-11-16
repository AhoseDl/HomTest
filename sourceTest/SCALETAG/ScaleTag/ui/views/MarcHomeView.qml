import QtQuick 2.9
import QtQuick.Window 2.9
import assets 1.0
import graphics 1.0
import VTK 9.3
import components 1.0
import Qt.labs.platform 1.1 as Platform
//import QtQuick.Dialogs
import QtQuick.Controls 2.3
//import V 1.0
import QtQuick.Controls.Imagine 2.3
import QtQuick.Layouts 1.3

import SCT 1.0
//import QtQml

Item{
    id:marcHomeView
    //anchors.fill:parent
    clip:true

    Rectangle{
        id: topSideMarcView
        color:"#efefef"
        border.width: 1
        border.color: "#ffffff"
        height:35
        anchors{
            top:parent.top
            right:parent.right
            left:parent.left
        }
        /*Text{
            text: "Tools for visualization"
            font.pixelSize: 40
            anchors.centerIn:parent
        }*/

        RowLayout{
            anchors.centerIn:parent
            spacing: 18
        Rectangle{
            id : textRectangle
            width:75
            height: 35
            color:"#efefef"
            /*anchors.verticalCenter*/Layout.alignment: /*parent.verticalCenter*/Qt.AlignVCenter

        Text{
        text:"Current model: "
        font.pixelSize: 14

        //font.italic:true
        font.family :"DejaVu Sans"
        anchors.centerIn:parent
        }}

////////////////////////////////////////////////


        ComboBox {
           // signal indexChanged(int ind)
            id:combo
            model:solaris.ui_ObjectFactory.ui_project.ui_ModelsNames //Just for convenience to have popup even if one item

            implicitHeight: 27
            implicitWidth: 110
            property real radius: 4
            property color backgroundColor: "#ffffff"/*"#2a2d36"*/
            property color checkedColor: /*AppStyle.popupBackground*//*"#000000"*/"transparent"
            delegate: ItemDelegate{
                id:itemDelegate
                width: combo.implicitWidth * 1.2
                height: combo.implicitHeight
                hoverEnabled: true
                focus: true
                background: Rectangle{
                    anchors.fill: parent
                    radius: 4
                    color: itemDelegate.hovered ? /*combo.backgroundColor*/"#333333" : "transparent"

 anchors.verticalCenter:parent.center
                }
                RowLayout{
                    Layout.alignment: Qt.AlignVCenter
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    spacing: 5
                    Layout.leftMargin: 5
                    Layout.rightMargin: 5
                    Label{
                        opacity: 0.9
                        text:modelData //data to be printed for each item in popup
                        font.pixelSize: /*FontStyle.h3*/14
                        color: itemDelegate.hovered ? "#FFFFFF" : /*AppStyle.textColor*/"#000000"
                        Layout.fillWidth: true
                        verticalAlignment: Image.AlignVCenter
                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 5

                    }

        //            Image{
        //                visible: root.currentIndex === index
        //                source: "qrc:/Images/tick.svg"
        //                verticalAlignment: Image.AlignVCenter
        //                horizontalAlignment: Image.AlignRight
        //                Layout.alignment: Qt.AlignVCenter
        //                Layout.rightMargin: 10
        //            }
                }
            }//Fin delegate

            contentItem:Item{
//data to be displayed once combobox closed
                width: combo.background.width - combo.indicator.width - 5
                height: combo.background.height
                RowLayout{
                    anchors.fill: parent
                    spacing: 15
                    Label{
                        id:cItem
                        opacity: 0.9
                        clip:true
                        //text:solaris.ui_ObjectFactory.getModelsNamesList()[listView.currentIndex]
                        //text:solaris.ui_ObjectFactory.nameAt(listView.currentIndex)
                        text:solaris.ui_ObjectFactory.ui_project.ui_currentModelName
                        Component.onCompleted:console.log(combo.currentIndex)
                        //text: combo.displayText
                        //text:combo.currentText
                        font.pixelSize: /*FontStyle.h3*/14
                        Layout.fillWidth: true
                        verticalAlignment: Image.AlignVCenter
                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 5
                        color: "#000000"

                    }

//                    Binding{
//                        when:listView.isCurrentItem
//                        // @disable-check M16
//                        cItem.text:solaris.ui_ObjectFactory.getModelsNamesList()[listView.currentIndex]
//                    }
                }
            }//fin contentitem

            background: Rectangle{
                implicitWidth: combo.implicitWidth
                implicitHeight: combo.implicitHeight
                color: combo.down ? Qt.darker(combo.checkedColor,1.2) : combo.checkedColor
                radius: combo.radius
                border.width: combo.activeFocus ? 2 : 0.6
                border.color: combo.activeFocus ? /*AppStyle.appStyle : AppStyle.borderColor*/ "#000000" : "#000000"
            }

            popup: Popup{
                y:combo.height + 2
                width: combo.implicitWidth * 1.26
                implicitHeight: contentItem.implicitHeight > 250 ? 250 : contentItem.implicitHeight
                padding: 2

                onAboutToHide:{
                    //cItem.text=solaris.ui_ObjectFactory.ui_project.ui_ModelsNames[listView.currentIndex]  ;
                   // if(listView.currentIndex !== solaris.ui_ObjectFactory.ui_project.ui_currentModelIndex ){
                    solaris.ui_ObjectFactory.ui_project.setCurrentIndex(listView.currentIndex);
                    //}
                }

                contentItem: ListView{
                    id:listView
                    leftMargin: 3
                    implicitHeight: contentHeight
                    keyNavigationEnabled: true
                    model:combo.popup.visible ? combo.delegateModel : null
                    clip: true
                    focus: true
                    currentIndex: combo.highlightedIndex
                }

                background: Rectangle{
                    anchors.fill: parent
                    color: /*AppStyle.popupBackground*/"#ffffff"
                    radius: 3
                    border.width: 0.6
                    border.color: /*AppStyle.borderColor*/"#000000"
                    clip: true
                }

            }

        } //Fin combobox
        }
    }

/////////////////////////////////////


/*    Rectangle{
        id: rightSideMarcView
        width:Style.simulationButtonsBarWidth
        color:"#ffffff"
        anchors{
            top:topSideMarcView.bottom
            //right:parent.right
            bottom:parent.bottom
            right:marcHomeView.right
            //left:vtkwindow.right


        }
        SimulationButton{
            id:fileIcon
            buttonIcon: "\uf15b"
            tooltip: "Open *.dat file"
            buttonColor:"#B7950B"

            Platform.FileDialog{
                id: fileDialog

                title:"Choose the input file"
                nameFilters: ["Input file(*.dat)", "All files (*)"]
                folder:solaris.ui_ObjectFactory.ui_workDirectory.getDirectory()
                onAccepted: {
                    fileDialog.close();
                    var ss=file.toString().replace(/^(file:\/{2})/,"");
                    //workDirection.setWorkDir
                    //view.init(ss);
                    solaris.ui_ObjectFactory.ui_Visualization.init(ss);
                    console.log(file);
                    console.log(ss);
                }
            }
            onSimulationButtonClicked:{fileDialog.folder=solaris.ui_ObjectFactory.ui_workDirectory.getDirectory() ; fileDialog.open();}
        }

        SimulationButton{
            id:executeIcon
            anchors.top:fileIcon.bottom
            buttonIcon: "\uf04b"
            buttonColor: "#1e8449"
            tooltip: "Run"
        }
    }
*/

    //    Rectangle{
//        id: leftSideMarcView
//        width:40
//        color:"#009900"
//        anchors{
//            top:topSideMarcView.bottom
//            left:parent.left
//            bottom:parent.bottom
//        }
//        Text{
//            font.pixelSize: 40
//            rotation:-90
//            anchors.centerIn: parent

//            text: "SCALETAG"

//        }

//    }


    Item {
        id: renderItem
        anchors{
            top: topSideMarcView.bottom
            //left: leftSideMarcView.right
            left: parent.left
            //right: rightSideMarcView.left
            right: parent.right
            bottom: parent.bottom}


// @disable-check M300
        VTKRenderWindow{

            id:vtkwindow
            // objectName:vtkwindow
            // anchors.fill:renderItem
            //width:renderItem.width
            //height:renderItem.height
            /*anchors{
                top:renderItem.top
                left:renderItem.left
                right:renderItem.right
                bottom:renderItem.bottom
            }*/
            anchors.fill:parent
 //visible:false

        }
// @disable-check M300
        VTKRenderItem{
            objectName:"visual"
            //x:0
            //y:0
            anchors.fill:renderItem
            //width:vtkwindow.width
            //height:vtkwindow.height
            renderWindow:vtkwindow

        }

    }


//    Connections{
//        target:solaris.ui_ObjectFactory
//        function onModelsNamesListChanged(modelList){
//            //listView.model=solaris.ui_ObjectFactory.getModelsNamesList();
//            combo.model=solaris.ui_ObjectFactory.getModelsNamesList();}
//    } //Not necessary

}
