import QtQuick 2.9
import QtQuick.Window 2.9
import QtQuick.Controls 2.9 /*2.9*/
import components 1.0
import assets 1.0
//import QtQuick.Dialogs
import Qt.labs.platform 1.1 as Platform
//import QtGraphicalEffects 1.12
Window{
    id: mainWindow
    objectName: "Nothing to say to Major Tom!"
    width: 900
    height: 850
    visible: true
    title: qsTr("ScaleTag")
//    minimumWidth: 900
//    minimumHeight: 850

StackView{
    id: contentFrame
    //clip:true
    anchors.fill:parent
   initialItem: "qrc:/views/WelcomeView.qml"

  // //initialItem: "qrc:/views/Graphics.qml"

//   replaceEnter: Transition {
//             PropertyAnimation{
//                 property: "opacity"
//                 from: /*1*/99
//                 to: /*1*/100
//                 duration: 300
//             }
//    }

//    replaceExit: Transition {
//             PropertyAnimation{
//                 property: "opacity"
//                 from: /*0*/0
//                 to: /*0*/0
//                 duration: 250
//             }
//    }


      replaceEnter: Transition {PropertyAnimation{
property: "visible" ; from: 1 ; to: 1 ; duration: 2
                }
          PropertyAnimation{
          property: "opacity" ; from: 1 ; to: 1 ; duration: 0
                          }

       }

       replaceExit: Transition {
PropertyAnimation{ property: "opacity" ; from: 0 ; to: 0 ; duration: 0
                }
PropertyAnimation{property: "visible" ; from: 0 ; to: 2 ; duration: 2
                }
}




//   replaceEnter: Transition {
//       id: enterTrans
//       SequentialAnimation {
//           // 1. Force l'élément entrant à être invisible au départ (immédiat)
//           PropertyAction {
//               target: enterTrans.ViewTransition.item
//               property: "opacity"
//               value: 0
//           }

//           // 2. Attend 500 ms (ou la durée de votre choix)
//           PauseAnimation { duration: 500 }

//           // 3. Rend l'élément visible instantanément à la fin de la pause
//           PropertyAction {
//               target: enterTrans.ViewTransition.item
//               property: "opacity"
//               value: 1
//           }
//       }
//   }

//   replaceExit: Transition {
//       id: exitTrans
//       // Rend l'élément sortant invisible immédiatement sans attendre
//       PropertyAction {
//           target: exitTrans.ViewTransition.item
//           property: "opacity"
//           value: 0
//       }
//   }

}
}
