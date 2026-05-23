import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1000
    height: 700
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: "VocabMemster 2.0"
    // 移除 GTK 默认标题栏，使用自定义样式
    color: bgPrimary

    // 现代简约配色方案
    property bool darkMode: true
    property color bgPrimary: darkMode ? "#0f172a" : "#f8fafc"
    property color bgSecondary: darkMode ? "#1e293b" : "#f1f5f9"
    property color cardBg: darkMode ? "#1e293b" : "#ffffff"
    property color textPrimary: darkMode ? "#f1f5f9" : "#0f172a"
    property color textSecondary: darkMode ? "#94a3b8" : "#475569"
    property color borderColor: darkMode ? "#334155" : "#e2e8f0"
    property color accentColor: "#6366f1"
    property color successColor: "#10b981"
    property color warningColor: "#f59e0b"
    property color errorColor: "#ef4444"
    property color infoColor: "#0ea5e9"

    Material.theme: darkMode ? Material.Dark : Material.Light
    Material.accent: accentColor
    Material.primary: accentColor

    // 自定义标题栏
    header: Rectangle {
        width: parent.width
        height: 48
        color: cardBg
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            anchors.bottomMargin: 1
            spacing: 12
            
            LucideIcon {
                iconName: "book"
                iconColor: accentColor
                iconSize: 24
                Layout.alignment: Qt.AlignVCenter
            }
            
            Label {
                text: "VocabMemster"
                color: textPrimary
                font.pixelSize: 16
                font.bold: true
                Layout.fillWidth: true
            }
            
            Item { Layout.fillWidth: true }
            
            Button {
                flat: true
                icon.name: ""
                contentItem: LucideIcon {
                    iconName: window.darkMode ? "moon" : "sun"
                    iconColor: accentColor
                    iconSize: 18
                }
                onClicked: darkMode = !darkMode
                Layout.alignment: Qt.AlignVCenter
            }
        }
        
        // 底部边框线
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: borderColor
        }
    }

    function feedbackColor(kind) {
        if (kind === "success") return successColor
        if (kind === "error") return errorColor
        if (kind === "answer") return infoColor
        return textSecondary
    }

    function addEditorWord(w, p, m, e) {
        editorModel.append({"word": w || "", "pos": p || "", "meaning": m || "", "example": e || ""})
    }

    function editorItems() {
        const items = []
        for (let i = 0; i < editorModel.count; ++i) {
            items.push(editorModel.get(i))
        }
        return items
    }

    Connections {
        target: appController
        function onToastRequested(message, kind) {
            toast.kind = kind
            toast.text = message
            toast.open()
        }
    }

    ListModel { id: editorModel }

    // ===== 文件对话框 =====
    FileDialog {
        id: openDialog
        title: "选择单词库文件"
        fileMode: FileDialog.OpenFile
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: appController.loadWordBook(selectedFile)
    }

    FileDialog {
        id: exportDialog
        title: "导出错词本"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text Files (*.txt)"]
        onAccepted: appController.exportWrongWords(selectedFile)
    }

    FileDialog {
        id: editorImportDialog
        title: "导入词库"
        fileMode: FileDialog.OpenFile
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: {
            const words = appController.loadEditorWords(selectedFile)
            if (words.length > 0) {
                editorModel.clear()
                for (let i = 0; i < words.length; ++i) editorModel.append(words[i])
                editorDialog.open()
            }
        }
    }

    FileDialog {
        id: editorSaveDialog
        title: "保存词库"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text Files (*.txt)"]
        onAccepted: appController.saveEditorWords(selectedFile, editorItems())
    }

    MessageDialog {
        id: aboutDialog
        title: "关于 VocabMemster 2.0"
        text: "现代化单词学习工具\n\n* 使用 C++ 逻辑层 + QML 表现层重写\n* 支持词库导入导出\n* 拼写练习与错词本\n* 深色/浅色主题\n* 快捷键支持"
    }

    MessageDialog {
        id: helpDialog
        title: "使用说明"
        text: "快速开始: \n1. 点击打开词库导入 .txt 文件\n2. 输入英文单词，按 Enter 或点击提交\n3. 错误自动加入错词本\n\n快捷键:\n* Alt+O: 打开词库\n* Alt+S: 查看答案\n* Ctrl+Tab: 下一个单词\n* Ctrl+Shift+D: 切换主题"
    }

    MessageDialog {
        id: debugDialog
        title: "调试信息"
        text: ""
    }

    // ===== 快捷键 =====
    Shortcut { sequence: "Alt+O"; onActivated: openDialog.open() }
    Shortcut { sequence: "Alt+S"; onActivated: appController.revealAnswer() }
    Shortcut { sequence: "Ctrl+Tab"; onActivated: appController.nextWord() }
    Shortcut { sequence: "Ctrl+Shift+D"; onActivated: darkMode = !darkMode }

    // ===== 自定义按钮样式 =====
    component StyledButton: Button {
        property color bgColor: accentColor
        background: Rectangle {
            radius: 8
            color: bgColor
            border.color: bgColor
            opacity: parent.hovered ? 0.9 : 1.0
        }
    }

    // ===== 主布局 =====
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 16
        anchors.topMargin: 16
        spacing: 16

        // ===== 侧边栏 =====
        Rectangle {
            id: sidebar
            Layout.preferredWidth: 280
            Layout.fillHeight: true
            radius: 16
            color: cardBg
            border.color: borderColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16

                // 应用标题
                ColumnLayout {
                    spacing: 8
                    RowLayout {
                        spacing: 8
                        LucideIcon { iconName: "book"; iconColor: accentColor; iconSize: 28 }
                        Label {
                            text: "VocabMemster"
                            color: textPrimary
                            font.pixelSize: 20
                            font.bold: true
                        }
                    }
                    Label {
                        text: "智能单词学习"
                        color: textSecondary
                        font.pixelSize: 12
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: borderColor
                }

                // 主要按钮
                Button {
                    Layout.fillWidth: true
                    text: "打开词库"
                    font.bold: true
                    contentItem: RowLayout {
                        spacing: 8
                        anchors.centerIn: parent
                        LucideIcon { iconName: "folder"; iconColor: "white"; iconSize: 18 }
                        Text { text: parent.parent.text; color: "white" }
                    }
                    background: Rectangle { radius: 8; color: accentColor }
                    onClicked: openDialog.open()
                }

                Button {
                    Layout.fillWidth: true
                    text: "编辑词库"
                    contentItem: RowLayout {
                        spacing: 8
                        anchors.centerIn: parent
                        LucideIcon { iconName: "edit"; iconColor: palette.buttonText; iconSize: 18 }
                        Text { text: parent.parent.text; color: palette.buttonText }
                    }
                    onClicked: { editorModel.clear(); addEditorWord("", "", "", ""); editorDialog.open() }
                }

                Button {
                    Layout.fillWidth: true
                    text: "错词本 (" + appController.wrongCount + ")"
                    contentItem: RowLayout {
                        spacing: 8
                        anchors.centerIn: parent
                        LucideIcon { iconName: "alert-circle"; iconColor: palette.buttonText; iconSize: 18 }
                        Text { text: parent.parent.text; color: palette.buttonText }
                    }
                    onClicked: wrongDrawer.open()
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: borderColor
                }

                // 统计信息
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    StatCard {
                        icon: "bar-chart-3"
                        label: "总单词"
                        value: appController.totalCount
                        color: accentColor
                    }

                    StatCard {
                        icon: "check-circle-2"
                        label: "已掌握"
                        value: appController.masteredCount
                        color: successColor
                    }

                    StatCard {
                        icon: "alert-circle"
                        label: "错词"
                        value: appController.wrongCount
                        color: errorColor
                    }

                    StatCard {
                        icon: "clock"
                        label: "剩余"
                        value: appController.remainingCount
                        color: warningColor
                    }
                }

                Item { Layout.fillHeight: true }

                // 帮助和关于
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Button {
                        Layout.fillWidth: true
                        text: "帮助"
                        font.pixelSize: 11
                        contentItem: RowLayout {
                            anchors.centerIn: parent
                            spacing: 4
                            LucideIcon { iconName: "help-circle"; iconColor: palette.buttonText; iconSize: 16 }
                            Text { text: parent.parent.text; color: palette.buttonText }
                        }
                        onClicked: helpDialog.open()
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "关于"
                        font.pixelSize: 11
                        contentItem: RowLayout {
                            anchors.centerIn: parent
                            spacing: 4
                            LucideIcon { iconName: "info"; iconColor: palette.buttonText; iconSize: 16 }
                            Text { text: parent.parent.text; color: palette.buttonText }
                        }
                        onClicked: aboutDialog.open()
                    }
                }
            }
        }

        // ===== 主内容区 =====
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            // 头部进度条
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                radius: 16
                color: cardBg
                border.color: borderColor
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 20

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        Label {
                            text: appController.loaded ? "📖 今日练习" : "👋 欢迎使用"
                            color: textPrimary
                            font.pixelSize: 24
                            font.bold: true
                        }
                        Label {
                            text: appController.statusText
                            color: textSecondary
                            font.pixelSize: 12
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    ProgressRing {
                        size: 70
                        lineWidth: 6
                        value: appController.progress
                        foreground: appController.completed ? successColor : accentColor
                        backgroundColor: darkMode ? "#334155" : "#e2e8f0"
                    }

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: Math.round(appController.progress * 100) + "%"
                            color: textPrimary
                            font.pixelSize: 24
                            font.bold: true
                        }
                        Label {
                            text: "进度"
                            color: textSecondary
                            font.pixelSize: 11
                        }
                    }
                }
            }

            // 内容区
            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: appController.completed ? 2 : (appController.loaded ? 1 : 0)

                EmptyState {}
                StudyPage {}
                CompletionPage {}
            }
        }
    }

    // ===== 错词本抽屉 =====
    Drawer {
        id: wrongDrawer
        width: Math.min(window.width * 0.4, 480)
        height: window.height
        edge: Qt.RightEdge
        modal: true
        background: Rectangle { color: cardBg; border.color: borderColor }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            RowLayout {
                Layout.fillWidth: true
                Label {
                    text: "❌ 错词本"
                    color: textPrimary
                    font.pixelSize: 18
                    font.bold: true
                    Layout.fillWidth: true
                }
                Button {
                    text: "✕"
                    flat: true
                    onClicked: wrongDrawer.close()
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Button {
                    Layout.fillWidth: true
                    text: "💾 导出"
                    enabled: appController.wrongCount > 0
                    onClicked: exportDialog.open()
                }
                Button {
                    Layout.fillWidth: true
                    text: "🗑️ 清空"
                    enabled: appController.wrongCount > 0
                    onClicked: appController.clearWrongWords()
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: borderColor }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: appController.wrongWordsModel
                spacing: 10
                clip: true

                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 12
                    color: bgSecondary
                    border.color: borderColor
                    implicitHeight: contentLayout.implicitHeight + 20

                    ColumnLayout {
                        id: contentLayout
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 6

                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                text: word
                                color: textPrimary
                                font.pixelSize: 16
                                font.bold: true
                                Layout.fillWidth: true
                            }
                            Label {
                                text: pos ? "[" + pos + "]" : ""
                                color: accentColor
                                font.bold: true
                            }
                        }

                        Label {
                            text: meaning
                            color: textPrimary
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                            font.pixelSize: 13
                        }

                        Label {
                            text: example ? "例：" + example : ""
                            color: textSecondary
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                            font.pixelSize: 11
                        }
                    }
                }

                Label {
                    anchors.centerIn: parent
                    visible: appController.wrongCount === 0
                    text: "✨ 暂无错词"
                    color: textSecondary
                    font.pixelSize: 14
                }
            }
        }
    }

    // ===== 编辑对话框 =====
    Dialog {
        id: editorDialog
        title: "📝 词库编辑器"
        modal: true
        width: Math.min(window.width - 60, 900)
        height: Math.min(window.height - 60, 620)
        standardButtons: Dialog.NoButton
        background: Rectangle { radius: 16; color: cardBg; border.color: borderColor }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            RowLayout {
                Layout.fillWidth: true
                Label {
                    text: "词库编辑器"
                    color: textPrimary
                    font.pixelSize: 18
                    font.bold: true
                    Layout.fillWidth: true
                }
                Button { text: "📥 导入"; onClicked: editorImportDialog.open() }
                Button { text: "➕ 新增"; onClicked: addEditorWord("", "", "", "") }
                StyledButton { text: "💾 保存"; bgColor: accentColor; onClicked: editorSaveDialog.open() }
                Button { text: "✕"; flat: true; onClicked: editorDialog.close() }
            }

            Label {
                text: "格式：单词 | 词性 | 中文释义 | 例句"
                color: textSecondary
                font.pixelSize: 11
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: editorModel
                spacing: 12
                clip: true

                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 12
                    color: bgSecondary
                    border.color: borderColor
                    implicitHeight: editorLayout.implicitHeight + 16

                    GridLayout {
                        id: editorLayout
                        anchors.fill: parent
                        anchors.margins: 12
                        columns: 5
                        columnSpacing: 8
                        rowSpacing: 8

                        TextField {
                            Layout.columnSpan: 2
                            Layout.fillWidth: true
                            placeholderText: "单词"
                            text: word
                            onTextChanged: editorModel.setProperty(index, "word", text)
                        }

                        TextField {
                            Layout.columnSpan: 1
                            Layout.fillWidth: true
                            placeholderText: "词性"
                            text: pos
                            onTextChanged: editorModel.setProperty(index, "pos", text)
                        }

                        TextField {
                            Layout.columnSpan: 1
                            Layout.fillWidth: true
                            placeholderText: "释义"
                            text: meaning
                            onTextChanged: editorModel.setProperty(index, "meaning", text)
                        }

                        Button {
                            Layout.columnSpan: 1
                            text: "🗑️"
                            onClicked: editorModel.remove(index)
                        }

                        TextArea {
                            Layout.columnSpan: 5
                            Layout.fillWidth: true
                            Layout.preferredHeight: 60
                            placeholderText: "例句"
                            text: example
                            wrapMode: TextEdit.WordWrap
                            onTextChanged: editorModel.setProperty(index, "example", text)
                        }
                    }
                }
            }
        }
    }

    // ===== 提示消息 =====
    Popup {
        id: toast
        property string kind: "info"
        property alias text: toastLabel.text
        x: (window.width - width) / 2
        y: 24
        width: Math.min(520, implicitWidth)
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose
        padding: 0
        background: Rectangle {
            radius: 12
            color: {
                if (toast.kind === "error") return errorColor
                if (toast.kind === "success") return successColor
                return accentColor
            }
            opacity: 0.95
        }
        contentItem: Label {
            id: toastLabel
            color: "white"
            font.pixelSize: 13
            font.bold: true
            padding: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
        Timer { id: toastTimer; interval: 2400; onTriggered: toast.close() }
        onOpened: toastTimer.restart()
    }

    // ===== 组件定义 =====
    component StatCard: Rectangle {
        property string icon
        property string label
        property int value
        property color color

        Layout.fillWidth: true
        height: 50
        radius: 12
        color: Qt.rgba(bgSecondary.r, bgSecondary.g, bgSecondary.b, 0.5)
        border.color: borderColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            LucideIcon {
                iconName: icon
                iconColor: color
                iconSize: 24
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Label {
                    text: label
                    color: textSecondary
                    font.pixelSize: 10
                }

                Label {
                    text: value
                    color: textPrimary
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
    }

    component EmptyState: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.7, 580)
            spacing: 20

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "📚"
                font.pixelSize: 80
            }

            Label {
                Layout.fillWidth: true
                text: "开始学习"
                color: textPrimary
                font.pixelSize: 28
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                Layout.fillWidth: true
                text: "打开一个单词库，开始高效的英文学习之旅"
                color: textSecondary
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12

                StyledButton {
                    text: "📂 打开词库"
                    bgColor: accentColor
                    onClicked: openDialog.open()
                }

                Button {
                    text: "✏️ 新建词库"
                    onClicked: { editorModel.clear(); addEditorWord("example", "n.", "例子", "This is an example."); editorDialog.open() }
                }
            }
        }
    }

    component StudyPage: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 16
                color: cardBg
                border.color: borderColor

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 28
                    spacing: 20

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        Label {
                            text: appController.currentPos ? "[" + appController.currentPos + "]" : ""
                            color: accentColor
                            font.pixelSize: 14
                            font.bold: true
                            padding: 8
                            background: Rectangle {
                                radius: 8
                                color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.1)
                                border.color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.3)
                            }
                        }

                        Item { Layout.fillWidth: true }

                        Label {
                            text: "尝试：" + appController.attemptCount
                            color: appController.attemptCount > 0 ? warningColor : textSecondary
                            font.bold: true
                        }
                    }

                    Item { Layout.preferredHeight: 12 }

                    Label {
                        Layout.fillWidth: true
                        text: appController.currentMeaning || "请加载单词库"
                        color: textPrimary
                        font.pixelSize: 40
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        Layout.fillWidth: true
                        text: appController.currentExample ? "📌 " + appController.currentExample : ""
                        color: textSecondary
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Item { Layout.fillHeight: true }

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 12
                        color: bgSecondary
                        border.color: borderColor
                        implicitHeight: answerArea.implicitHeight + 24

                        ColumnLayout {
                            id: answerArea
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 12

                            TextField {
                                id: answerField
                                Layout.fillWidth: true
                                enabled: appController.inputEnabled
                                placeholderText: "输入英文单词，按 Enter 提交"
                                font.pixelSize: 16
                                selectByMouse: true
                                onAccepted: {
                                    appController.submitAnswer(text)
                                    if (appController.inputEnabled) selectAll()
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 12

                                StyledButton {
                                    Layout.fillWidth: true
                                    text: "✅ 提交"
                                    bgColor: accentColor
                                    enabled: appController.inputEnabled
                                    onClicked: {
                                        appController.submitAnswer(answerField.text)
                                        if (appController.inputEnabled) answerField.selectAll()
                                    }
                                }

                                Button {
                                    Layout.fillWidth: true
                                    text: "👁️ 看答案"
                                    enabled: appController.inputEnabled
                                    onClicked: appController.revealAnswer()
                                }

                                StyledButton {
                                    Layout.fillWidth: true
                                    text: "⏭️ 下一个"
                                    bgColor: successColor
                                    onClicked: { answerField.text = ""; appController.nextWord(); answerField.forceActiveFocus() }
                                }
                            }

                            Label {
                                Layout.fillWidth: true
                                text: appController.feedbackText
                                color: feedbackColor(appController.feedbackKind)
                                font.pixelSize: 14
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                visible: text.length > 0
                            }
                        }
                    }
                }
            }
        }
    }

    component CompletionPage: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.7, 600)
            spacing: 24

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: "🎉"
                font.pixelSize: 100
            }

            Label {
                Layout.fillWidth: true
                text: "恭喜你！"
                color: textPrimary
                font.pixelSize: 36
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                Layout.fillWidth: true
                text: "你已掌握全部 " + appController.totalCount + " 个单词\n错词本中有 " + appController.wrongCount + " 个单词需继续复习"
                color: textSecondary
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12

                StyledButton {
                    text: "🔄 重新开始"
                    bgColor: accentColor
                    onClicked: appController.resetProgress()
                }

                Button {
                    text: "📋 查看错词本"
                    enabled: appController.wrongCount > 0
                    onClicked: wrongDrawer.open()
                }

                Button {
                    text: "💾 导出错词"
                    enabled: appController.wrongCount > 0
                    onClicked: exportDialog.open()
                }
            }
        }
    }

    component ProgressRing: Item {
        property real value: 0
        property int size: 64
        property int lineWidth: 8
        property color foreground: accentColor
        property color backgroundColor: "#334155"

        width: size
        height: size
        onValueChanged: canvas.requestPaint()
        onForegroundChanged: canvas.requestPaint()

        Canvas {
            id: canvas
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                var cx = width / 2, cy = height / 2
                var r = Math.min(width, height) / 2 - lineWidth

                // 背景圆
                ctx.lineWidth = lineWidth
                ctx.lineCap = "round"
                ctx.strokeStyle = backgroundColor
                ctx.beginPath()
                ctx.arc(cx, cy, r, 0, Math.PI * 2)
                ctx.stroke()

                // 进度圆
                ctx.strokeStyle = foreground
                ctx.beginPath()
                ctx.arc(cx, cy, r, -Math.PI / 2, -Math.PI / 2 + Math.PI * 2 * Math.max(0, Math.min(1, value)))
                ctx.stroke()
            }
        }
    }
}
