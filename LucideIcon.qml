import QtQuick
import QtQuick.Controls

Canvas {
    id: lucideIcon
    property string iconName: "book"
    property color iconColor: "#ffffff"
    property int iconSize: 24
    
    width: iconSize
    height: iconSize
    
    onPaint: {
        var ctx = getContext("2d")
        ctx.clearRect(0, 0, width, height)
        ctx.strokeStyle = iconColor
        ctx.lineWidth = 1.5
        ctx.lineCap = "round"
        ctx.lineJoin = "round"
        ctx.fillStyle = "none"
        
        // 缩放坐标以适应不同的图标大小
        var scale = Math.min(width, height) / 24
        ctx.translate(width / 2, height / 2)
        ctx.scale(scale, scale)
        ctx.translate(-12, -12)
        
        switch(lucideIcon.iconName) {
            case "book":
                ctx.beginPath()
                ctx.moveTo(4, 19.5)
                ctx.quadraticCurveTo(4, 21, 6.5, 21)
                ctx.lineTo(20, 21)
                ctx.lineTo(20, 2)
                ctx.lineTo(6.5, 2)
                ctx.quadraticCurveTo(4, 2, 4, 4.5)
                ctx.closePath()
                ctx.stroke()
                break
            case "folder":
                ctx.beginPath()
                ctx.moveTo(22, 19)
                ctx.lineTo(2, 19)
                ctx.lineTo(2, 5)
                ctx.lineTo(10, 5)
                ctx.lineTo(12, 2)
                ctx.lineTo(22, 2)
                ctx.closePath()
                ctx.stroke()
                break
            case "edit":
                ctx.beginPath()
                ctx.moveTo(17, 3)
                ctx.lineTo(21, 7)
                ctx.lineTo(7, 21)
                ctx.lineTo(3, 21)
                ctx.lineTo(3, 17)
                ctx.closePath()
                ctx.stroke()
                break
            case "x":
                ctx.beginPath()
                ctx.moveTo(6, 6)
                ctx.lineTo(18, 18)
                ctx.moveTo(18, 6)
                ctx.lineTo(6, 18)
                ctx.stroke()
                break
            case "bar-chart-3":
                ctx.beginPath()
                ctx.moveTo(3, 3)
                ctx.lineTo(3, 21)
                ctx.lineTo(21, 21)
                ctx.moveTo(3, 15)
                ctx.lineTo(7, 15)
                ctx.lineTo(7, 21)
                ctx.moveTo(12, 9)
                ctx.lineTo(16, 9)
                ctx.lineTo(16, 21)
                ctx.stroke()
                break
            case "check-circle-2":
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(8, 12)
                ctx.lineTo(11, 15)
                ctx.lineTo(16, 9)
                ctx.stroke()
                break
            case "alert-circle":
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12, 8.5, 0.5, 0, Math.PI * 2)
                ctx.fill()
                ctx.beginPath()
                ctx.moveTo(12, 11)
                ctx.lineTo(12, 15)
                ctx.stroke()
                break
            case "clock":
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12, 6)
                ctx.lineTo(12, 12)
                ctx.lineTo(16, 14)
                ctx.stroke()
                break
            case "help-circle":
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.fillStyle = iconColor
                ctx.font = "11px Arial"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.fillText("?", 12, 13)
                break
            case "info":
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12, 7, 0.5, 0, Math.PI * 2)
                ctx.fill()
                ctx.beginPath()
                ctx.moveTo(12, 10)
                ctx.lineTo(12, 15)
                ctx.stroke()
                break
            case "download":
                ctx.beginPath()
                ctx.moveTo(12, 2)
                ctx.lineTo(12, 16)
                ctx.moveTo(5, 10)
                ctx.lineTo(12, 16)
                ctx.lineTo(19, 10)
                ctx.moveTo(2, 20)
                ctx.lineTo(22, 20)
                ctx.stroke()
                break
            case "plus":
                ctx.beginPath()
                ctx.moveTo(12, 5)
                ctx.lineTo(12, 19)
                ctx.moveTo(5, 12)
                ctx.lineTo(19, 12)
                ctx.stroke()
                break
            case "save":
                ctx.beginPath()
                ctx.moveTo(19, 21)
                ctx.lineTo(5, 21)
                ctx.quadraticCurveTo(2, 21, 2, 18)
                ctx.lineTo(2, 5)
                ctx.lineTo(22, 5)
                ctx.lineTo(22, 18)
                ctx.quadraticCurveTo(22, 21, 19, 21)
                ctx.moveTo(7, 3)
                ctx.lineTo(7, 7)
                ctx.lineTo(17, 7)
                ctx.lineTo(17, 3)
                ctx.stroke()
                break
            case "trash-2":
                ctx.beginPath()
                ctx.moveTo(3, 6)
                ctx.lineTo(21, 6)
                ctx.moveTo(8, 6)
                ctx.lineTo(8, 20)
                ctx.quadraticCurveTo(8, 21, 9, 21)
                ctx.lineTo(15, 21)
                ctx.quadraticCurveTo(16, 21, 16, 20)
                ctx.lineTo(16, 6)
                ctx.moveTo(10, 11)
                ctx.lineTo(10, 17)
                ctx.moveTo(14, 11)
                ctx.lineTo(14, 17)
                ctx.moveTo(5, 6)
                ctx.lineTo(6, 3)
                ctx.lineTo(18, 3)
                ctx.lineTo(19, 6)
                ctx.stroke()
                break
            case "eye":
                ctx.beginPath()
                ctx.arc(12, 12, 8, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12, 12, 3, 0, Math.PI * 2)
                ctx.stroke()
                break
            case "rotate-cw":
                ctx.beginPath()
                ctx.arc(12, 12, 9, 0, Math.PI * 1.5)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(20, 4)
                ctx.lineTo(16, 4)
                ctx.lineTo(16, 8)
                ctx.stroke()
                break
            case "moon":
                ctx.beginPath()
                ctx.arc(13, 13, 9, -Math.PI * 0.25, Math.PI * 0.25)
                ctx.stroke()
                break
            case "sun":
                ctx.beginPath()
                ctx.arc(12, 12, 8, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12, 1)
                ctx.lineTo(12, 3)
                ctx.moveTo(12, 21)
                ctx.lineTo(12, 23)
                ctx.moveTo(1, 12)
                ctx.lineTo(3, 12)
                ctx.moveTo(21, 12)
                ctx.lineTo(23, 12)
                ctx.stroke()
                break
        }
    }
}
