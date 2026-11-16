import QtQuick 2.7
import QtQuick.Window 2.7
import VTK 9.3

Window{
    id:win
    title:"Viewport-ScaleTag"
objectName:"zigzag"
height: 750
width: 800
visible:true

/*Rectangle{
id: rect
anchors.fill:parent
color:"blue"}*/


VTKRenderWindow{
    id: vtkWindow
  //  width:400
    //height:400
    anchors.fill: parent
}

VTKRenderItem{
    objectName:"visual"
//    x:0
//    y:0
//    width:900
//    height:850
renderWindow: vtkWindow
anchors.fill: parent}

}
