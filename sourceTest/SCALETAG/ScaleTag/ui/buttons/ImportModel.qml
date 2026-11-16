import QtQuick 2.9
import QtQuick.Window 2.9
import assets 1.0
import graphics 1.0
//import VTK 9.3
import components 1.0
import Qt.labs.platform 1.1 as Platform

import QtQuick.Controls 2.3

import QtQuick.Controls.Imagine 2.3
import QtQuick.Layouts 1.3

import SCT 1.0


   // Rectangle
   Rectangle{
       //width: Style.simulationButtonWidth
      // height: Style.simulationButtonHeight
   visible:true
        id: rightSideMarcView
        width:40
        height:40
       // width:Style.simulationButtonsBarWidth
        //color:"#ffffff"
       // anchors{
         //   top:topSideMarcView.bottom
           // right:parent.right
          //  bottom:parent.bottom


        //}
        Column{
        SimulationButton{
            id:fileIcon
            buttonIcon: "\uf15b"
            tooltip: "Open *.dat file"
            buttonColor:"#B7950B"

            Platform.FileDialog{
                id: fileDialog

                title:"Choose the input file"
                nameFilters: ["Input file(*.dat)", "All files (*)"]
               // folder:solaris.ui_ObjectFactory.ui_workDirectory.getDirectory()
                onAccepted: {
                    fileDialog.close();
                    var ss=file.toString().replace(/^(file:\/{2})/,"");
                    //workDirection.setWorkDir
                    //view.init(ss);
                   // solaris.ui_ObjectFactory.ui_Visualization.init(ss);
                    console.log(file);
                    console.log(ss);
                }
            }
            //onSimulationButtonClicked://{fileDialog.folder=solaris.ui_ObjectFactory.ui_workDirectory.getDirectory() ; fileDialog.open();}
        }

        SimulationButton{
            id:executeIcon
            //anchors.top:fileIcon.bottom
            buttonIcon: "\uf04b"
            buttonColor: "#1e8449"
            tooltip: "Run"
            anchors.right:parent.right
        }}

    }
