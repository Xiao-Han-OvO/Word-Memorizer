import QtQuick
import QtQuick.Controls

// IconButton - 使用 lucide 风格图标的按钮
component IconButton: Button {
    property string iconName: "help"
    property int iconSize: 24
    property color iconColor: palette.buttonText
    
    palette.buttonText: window.textPrimary
    
    implicitWidth: Math.max(implicitBackgroundWidth + leftPadding + rightPadding, implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topPadding + bottomPadding, implicitContentHeight + topPadding + bottomPadding)
    
    contentItem: Row {
        spacing: 8
        anchors.centerIn: parent
        
        LucideIcon {
            iconName: parent.parent.iconName
            iconColor: parent.parent.iconColor
            iconSize: parent.parent.iconSize
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Text {
            text: parent.parent.text
            color: parent.parent.iconColor
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

// LucideIcon - lucide 风格的图标绘制
component LucideIcon: Canvas {
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
        
        drawIcon(ctx, iconName, width, height)
    }
    
    function drawIcon(ctx, name, w, h) {
        var cx = w / 2, cy = h / 2
        var scale = Math.min(w, h) / 24
        
        switch(name) {
            case "book":
                ctx.beginPath()
                ctx.moveTo(4*scale, 19.5*scale)
                ctx.quadraticCurveTo(4*scale, 21*scale, 6.5*scale, 21*scale)
                ctx.lineTo(20*scale, 21*scale)
                ctx.lineTo(20*scale, 2*scale)
                ctx.lineTo(6.5*scale, 2*scale)
                ctx.quadraticCurveTo(4*scale, 2*scale, 4*scale, 4.5*scale)
                ctx.lineTo(4*scale, 19.5*scale)
                ctx.stroke()
                break
            case "folder":
                ctx.beginPath()
                ctx.moveTo(22*scale, 19*scale)
                ctx.lineTo(2*scale, 19*scale)
                ctx.lineTo(2*scale, 5*scale)
                ctx.lineTo(10*scale, 5*scale)
                ctx.lineTo(12*scale, 2*scale)
                ctx.lineTo(22*scale, 2*scale)
                ctx.closePath()
                ctx.stroke()
                break
            case "edit":
                ctx.beginPath()
                ctx.moveTo(17*scale, 3*scale)
                ctx.lineTo(21*scale, 7*scale)
                ctx.lineTo(7*scale, 21*scale)
                ctx.lineTo(3*scale, 21*scale)
                ctx.lineTo(3*scale, 17*scale)
                ctx.closePath()
                ctx.stroke()
                break
            case "x":
                ctx.beginPath()
                ctx.moveTo(6*scale, 6*scale)
                ctx.lineTo(18*scale, 18*scale)
                ctx.moveTo(18*scale, 6*scale)
                ctx.lineTo(6*scale, 18*scale)
                ctx.stroke()
                break
            case "bar-chart-3":
                ctx.beginPath()
                ctx.moveTo(3*scale, 3*scale)
                ctx.lineTo(3*scale, 21*scale)
                ctx.lineTo(21*scale, 21*scale)
                ctx.moveTo(3*scale, 15*scale)
                ctx.lineTo(7*scale, 15*scale)
                ctx.lineTo(7*scale, 21*scale)
                ctx.moveTo(12*scale, 9*scale)
                ctx.lineTo(16*scale, 9*scale)
                ctx.lineTo(16*scale, 21*scale)
                ctx.stroke()
                break
            case "check-circle-2":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 10*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(8*scale, 12*scale)
                ctx.lineTo(11*scale, 15*scale)
                ctx.lineTo(16*scale, 9*scale)
                ctx.stroke()
                break
            case "alert-circle":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 10*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12*scale, 8.5*scale, 0.5*scale, 0, Math.PI * 2)
                ctx.fill()
                ctx.beginPath()
                ctx.moveTo(12*scale, 11*scale)
                ctx.lineTo(12*scale, 15*scale)
                ctx.stroke()
                break
            case "clock":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 10*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12*scale, 6*scale)
                ctx.lineTo(12*scale, 12*scale)
                ctx.lineTo(16*scale, 14*scale)
                ctx.stroke()
                break
            case "help-circle":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 10*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.font = Math.round(11*scale) + "px Arial"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.fillStyle = iconColor
                ctx.fillText("?", 12*scale, 12*scale)
                break
            case "info":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 10*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12*scale, 7*scale, 0.5*scale, 0, Math.PI * 2)
                ctx.fill()
                ctx.beginPath()
                ctx.moveTo(12*scale, 10*scale)
                ctx.lineTo(12*scale, 15*scale)
                ctx.stroke()
                break
            case "download":
                ctx.beginPath()
                ctx.moveTo(12*scale, 2*scale)
                ctx.lineTo(12*scale, 16*scale)
                ctx.moveTo(5*scale, 10*scale)
                ctx.lineTo(12*scale, 16*scale)
                ctx.lineTo(19*scale, 10*scale)
                ctx.moveTo(2*scale, 20*scale)
                ctx.lineTo(22*scale, 20*scale)
                ctx.stroke()
                break
            case "plus":
                ctx.beginPath()
                ctx.moveTo(12*scale, 5*scale)
                ctx.lineTo(12*scale, 19*scale)
                ctx.moveTo(5*scale, 12*scale)
                ctx.lineTo(19*scale, 12*scale)
                ctx.stroke()
                break
            case "save":
                ctx.beginPath()
                ctx.moveTo(19*scale, 21*scale)
                ctx.lineTo(5*scale, 21*scale)
                ctx.quadraticCurveTo(2*scale, 21*scale, 2*scale, 18*scale)
                ctx.lineTo(2*scale, 5*scale)
                ctx.lineTo(22*scale, 5*scale)
                ctx.lineTo(22*scale, 18*scale)
                ctx.quadraticCurveTo(22*scale, 21*scale, 19*scale, 21*scale)
                ctx.moveTo(7*scale, 3*scale)
                ctx.lineTo(7*scale, 7*scale)
                ctx.lineTo(17*scale, 7*scale)
                ctx.lineTo(17*scale, 3*scale)
                ctx.stroke()
                break
            case "trash-2":
                ctx.beginPath()
                ctx.moveTo(3*scale, 6*scale)
                ctx.lineTo(21*scale, 6*scale)
                ctx.moveTo(8*scale, 6*scale)
                ctx.lineTo(8*scale, 20*scale)
                ctx.quadraticCurveTo(8*scale, 21*scale, 9*scale, 21*scale)
                ctx.lineTo(15*scale, 21*scale)
                ctx.quadraticCurveTo(16*scale, 21*scale, 16*scale, 20*scale)
                ctx.lineTo(16*scale, 6*scale)
                ctx.moveTo(10*scale, 11*scale)
                ctx.lineTo(10*scale, 17*scale)
                ctx.moveTo(14*scale, 11*scale)
                ctx.lineTo(14*scale, 17*scale)
                ctx.moveTo(5*scale, 6*scale)
                ctx.lineTo(6*scale, 3*scale)
                ctx.lineTo(18*scale, 3*scale)
                ctx.lineTo(19*scale, 6*scale)
                ctx.stroke()
                break
            case "eye":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 8*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 3*scale, 0, Math.PI * 2)
                ctx.stroke()
                break
            case "chevron-right":
                ctx.beginPath()
                ctx.moveTo(9*scale, 6*scale)
                ctx.lineTo(15*scale, 12*scale)
                ctx.lineTo(9*scale, 18*scale)
                ctx.stroke()
                break
            case "rotate-cw":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 9*scale, 0, Math.PI * 1.5)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(20*scale, 4*scale)
                ctx.lineTo(16*scale, 4*scale)
                ctx.lineTo(16*scale, 8*scale)
                ctx.stroke()
                break
            case "moon":
                ctx.beginPath()
                ctx.arc(13*scale, 13*scale, 9*scale, -Math.PI * 0.25, Math.PI * 0.25)
                ctx.stroke()
                break
            case "sun":
                ctx.beginPath()
                ctx.arc(12*scale, 12*scale, 8*scale, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12*scale, 1*scale)
                ctx.lineTo(12*scale, 3*scale)
                ctx.moveTo(12*scale, 21*scale)
                ctx.lineTo(12*scale, 23*scale)
                ctx.moveTo(1*scale, 12*scale)
                ctx.lineTo(3*scale, 12*scale)
                ctx.moveTo(21*scale, 12*scale)
                ctx.lineTo(23*scale, 12*scale)
                ctx.stroke()
                break
            case "list":
                ctx.beginPath()
                ctx.moveTo(3*scale, 6*scale)
                ctx.arc(3.2*scale, 6*scale, 0.8*scale, 0, Math.PI * 2)
                ctx.moveTo(8*scale, 6*scale)
                ctx.lineTo(21*scale, 6*scale)
                ctx.moveTo(3*scale, 12*scale)
                ctx.arc(3.2*scale, 12*scale, 0.8*scale, 0, Math.PI * 2)
                ctx.moveTo(8*scale, 12*scale)
                ctx.lineTo(21*scale, 12*scale)
                ctx.moveTo(3*scale, 18*scale)
                ctx.arc(3.2*scale, 18*scale, 0.8*scale, 0, Math.PI * 2)
                ctx.moveTo(8*scale, 18*scale)
                ctx.lineTo(21*scale, 18*scale)
                ctx.stroke()
                break
        }
    }
}
