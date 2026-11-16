import QtQuick 2.9
import assets 1.0
import components 1.0
import SCT 1.0
//import QtQuick.Controls 2.9



Item{

    function setTimeout(delayTime, callback) {
        // Crée dynamiquement un objet Timer en mémoire
        var timer = Qt.createQmlObject("import QtQml 2.15; Timer {}", parent);
        timer.interval = delayTime;
        timer.repeat = false;

        // Connecte le signal à votre fonction
        timer.triggered.connect(function() {
            callback();
            timer.destroy(); // Libère la mémoire après exécution
        });

        timer.start();
    }


    id: welcomeView
    //anchors.fill:parent
//    clip:true
    Rectangle{
        id:welcomeRectangle
        anchors{
        top:welcomeView.top
        left:welcomeView.left
        right: welcomeView.right
         }
        height:(welcomeView.height)/2
    color:"#efefef"

    Text{
        id: welcomeContent
        anchors.centerIn:parent
        wrapMode:Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        font{
            //family: "Deja Vu Sans"
            family: "Times New Roman"
            pixelSize: 22
            bold: false
            italic: true
        }

        text: "This tool is dedicated to homogeneization using ScaleTag(a homemade tool), Abaqus\u00A9, Marc MSC\u00A9 or ANSYS\u00A9.
When using Abaqus, Marc MSC or ANSYS, we assume that you have these softwares on your computer and that they are reachable through your path environment variable."

        width:parent.width
    }}

    Rectangle{
        id:toSimulationTopRectangle
        color:"#efefef"
        anchors{
        top:welcomeRectangle.bottom
        left:welcomeView.left
        right: welcomeView.right
        bottom: welcomeView.bottom
         }

        Column{
            anchors.centerIn:parent
            ProjectTypeButton{
                id:toSimulation
            //welcomeViewColour:Style.welcomeViewColour //

            //buttonIcon: ""//"\uf061"
           // buttonIcon: "qrc:/logos/marc"
            buttonLabel: /*"GO"*/"ENTER"
            onProjectTypeButtonClicked: {
                //projectButtonsBar.iscollapsed=!projectButtonsBar.iscollapsed;
                //Style.projectTypeButtonIconWidth=0; Style.projectTypeButtonLabelWidth=0;
                //projectButtonsBar.buttonsBarVisibility=false;

           //contentFrame.replace(0); // 0 for StackView.Immediate
               // contentFrame.replace(contentFrame.inialItem,"qrc:/views/MarcHomeView.qml",0);

                //view.initialize();
                //solaris.ui_ObjectFactory.initializeView()
               // solaris.ui_ObjectFactory.ui_Visualization.initialize()
            mainWindow.visible=false; //very important so that setcentralwidget(MarcHomeView)  can work properly
                solaris.initObjectFactory();
            //contentFrame.replace(0); // 0 for StackView.Immediate
              //   contentFrame.replace(null,"qrc:/views/MarcHomeView.qml",0);

                setTimeout(1500/1500., function() {
                    //console.log("Exécuté après 1,5 seconde !");
                    contentFrame.replace(null,"qrc:/views/MarcHomeView.qml");
            solaris.ui_ObjectFactory.qmlWelcomeViewPassed();
                }
                )

//                contentFrame.replace(null,"qrc:/views/MarcHomeView.qml");
//        solaris.ui_ObjectFactory.qmlWelcomeViewPassed();

                /*binding in ObjectFatory - QObject::connect(this, &ObjectFactory::qmlWelcomeViewPassed,
                this, &ObjectFactory::showMainWindow);
                initialize() is called - visualization->initialize();*/
                //solaris.ui_ObjectFactory.getMainWindowController().showMainWindow()


                //solaris.ui_ObjectFactory.showMainWindow()

                //fileMenu.nowEnabled=true;
                //console.log("Establishing Marc Msc simulation context")
                //console.log("vrooom",mainWindow)
            }

        }
        }
    }
}


