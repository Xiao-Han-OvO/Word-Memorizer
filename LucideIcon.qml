import QtQuick
import QtQuick.Controls

// Lucide 图标组件 - 使用 SVG 路径绘制
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
        ctx.lineWidth = 2
        ctx.lineCap = "round"
        ctx.lineJoin = "round"
        ctx.fillStyle = "none"
        
        // 按照 24x24 的标准缩放
        var scale = Math.min(width, height) / 24
        ctx.scale(scale, scale)
        
        // 根据图标名称绘制不同的路径
        drawIcon(ctx, iconName)
    }
    
    function drawIcon(ctx, name) {
        switch(name) {
            case "book":
                // 书籍图标
                ctx.path("M4 19.5 C4 20.88 5.12 22 6.5 22 L20 22 L20 2 L6.5 2 C5.12 2 4 3.12 4 4.5 L4 19.5 Z")
                ctx.stroke()
                break
            case "folder":
                // 文件夹图标
                ctx.beginPath()
                ctx.moveTo(22, 19)
                ctx.lineTo(2, 19)
                ctx.lineTo(2, 5)
                ctx.lineTo(10, 5)
                ctx.lineTo(12, 2)
                ctx.lineTo(22, 2)
                ctx.lineTo(22, 19)
                ctx.stroke()
                break
            case "edit":
                // 编辑图标
                ctx.beginPath()
                ctx.moveTo(17, 3)
                ctx.lineTo(21, 7)
                ctx.lineTo(7, 21)
                ctx.lineTo(3, 21)
                ctx.lineTo(3, 17)
                ctx.lineTo(17, 3)
                ctx.stroke()
                break
            case "x":
                // 关闭图标
                ctx.beginPath()
                ctx.moveTo(6, 6)
                ctx.lineTo(18, 18)
                ctx.moveTo(18, 6)
                ctx.lineTo(6, 18)
                ctx.stroke()
                break
            case "chart":
                // 图表图标
                ctx.beginPath()
                ctx.moveTo(3, 21)
                ctx.lineTo(3, 3)
                ctx.lineTo(21, 3)
                ctx.moveTo(3, 15)
                ctx.lineTo(7, 15)
                ctx.lineTo(7, 21)
                ctx.moveTo(12, 9)
                ctx.lineTo(16, 9)
                ctx.lineTo(16, 21)
                ctx.stroke()
                break
            case "check":
                // 勾号圆形
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(8, 12)
                ctx.lineTo(11, 15)
                ctx.lineTo(16, 9)
                ctx.stroke()
                break
            case "alert":
                // 警告图标
                ctx.beginPath()
                ctx.moveTo(12, 2)
                ctx.lineTo(22, 19)
                ctx.lineTo(2, 19)
                ctx.lineTo(12, 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12, 15.5, 0.5, 0, Math.PI * 2)
                ctx.fill()
                break
            case "clock":
                // 时钟图标
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12, 6)
                ctx.lineTo(12, 12)
                ctx.lineTo(16, 14)
                ctx.stroke()
                break
            case "help":
                // 帮助图标
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.font = "16px Arial"
                ctx.textAlign = "center"
                ctx.textBaseline = "middle"
                ctx.fillStyle = iconColor
                ctx.fillText("?", 12, 12)
                break
            case "download":
                // 下载图标
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
                // 加号图标
                ctx.beginPath()
                ctx.moveTo(12, 5)
                ctx.lineTo(12, 19)
                ctx.moveTo(5, 12)
                ctx.lineTo(19, 12)
                ctx.stroke()
                break
            case "save":
                // 保存图标
                ctx.beginPath()
                ctx.moveTo(19, 21)
                ctx.lineTo(5, 21)
                ctx.lineTo(2, 18)
                ctx.lineTo(2, 5)
                ctx.lineTo(22, 5)
                ctx.lineTo(22, 18)
                ctx.lineTo(19, 21)
                ctx.moveTo(7, 3)
                ctx.lineTo(7, 7)
                ctx.lineTo(17, 7)
                ctx.lineTo(17, 3)
                ctx.stroke()
                break
            case "trash":
                // 垃圾桶图标
                ctx.beginPath()
                ctx.moveTo(3, 6)
                ctx.lineTo(21, 6)
                ctx.lineTo(20, 20)
                ctx.lineTo(4, 20)
                ctx.lineTo(3, 6)
                ctx.moveTo(10, 9)
                ctx.lineTo(10, 17)
                ctx.moveTo(14, 9)
                ctx.lineTo(14, 17)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(8, 6)
                ctx.lineTo(8, 3)
                ctx.lineTo(16, 3)
                ctx.lineTo(16, 6)
                ctx.stroke()
                break
            case "eye":
                // 眼睛图标
                ctx.beginPath()
                ctx.arc(12, 12, 10, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(12, 12, 3, 0, Math.PI * 2)
                ctx.stroke()
                break
            case "refresh":
                // 刷新图标
                ctx.beginPath()
                ctx.arc(12, 12, 9, 0.3, Math.PI * 1.7)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(16, 6)
                ctx.lineTo(20, 6)
                ctx.lineTo(20, 2)
                ctx.stroke()
                break
            case "moon":
                // 月亮图标
                ctx.beginPath()
                ctx.arc(12, 12, 10, -Math.PI * 0.3, Math.PI * 0.3)
                ctx.stroke()
                break
            case "sun":
                // 太阳图标
                ctx.beginPath()
                ctx.arc(12, 12, 8, 0, Math.PI * 2)
                ctx.stroke()
                ctx.beginPath()
                ctx.moveTo(12, 1)
                ctx.lineTo(12, 3)
                ctx.moveTo(12, 21)
                ctx.lineTo(12, 23)
                ctx.moveTo(4.22, 4.22)
                ctx.lineTo(5.64, 5.64)
                ctx.moveTo(18.36, 18.36)
                ctx.lineTo(19.78, 19.78)
                ctx.moveTo(1, 12)
                ctx.lineTo(3, 12)
                ctx.moveTo(21, 12)
                ctx.lineTo(23, 12)
                ctx.moveTo(4.22, 19.78)
                ctx.lineTo(5.64, 18.36)
                ctx.moveTo(18.36, 5.64)
                ctx.lineTo(19.78, 4.22)
                ctx.stroke()
                break
            case "list":
                // 列表图标
                ctx.beginPath()
                ctx.moveTo(8, 6)
                ctx.lineTo(21, 6)
                ctx.moveTo(8, 12)
                ctx.lineTo(21, 12)
                ctx.moveTo(8, 18)
                ctx.lineTo(21, 18)
                ctx.moveTo(3, 6)
                ctx.arc(3.2, 6, 0.2, 0, Math.PI * 2)
                ctx.moveTo(3, 12)
                ctx.arc(3.2, 12, 0.2, 0, Math.PI * 2)
                ctx.moveTo(3, 18)
                ctx.arc(3.2, 18, 0.2, 0, Math.PI * 2)
                ctx.stroke()
                break
        }
    }
}
