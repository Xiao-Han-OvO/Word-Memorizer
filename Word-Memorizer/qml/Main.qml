import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1180
    height: 760
    minimumWidth: 980
    minimumHeight: 640
    visible: true
    title: "VocabMemster"

    property bool darkMode: true
    property color bg0: darkMode ? "#09111f" : "#edf4ff"
    property color bg1: darkMode ? "#151a34" : "#f9fbff"
    property color card: darkMode ? "#182238" : "#ffffff"
    property color card2: darkMode ? "#202b45" : "#f4f7ff"
    property color text0: darkMode ? "#f8fbff" : "#172033"
    property color text1: darkMode ? "#b9c5d8" : "#647089"
    property color line: darkMode ? "#31405f" : "#dde6f5"
    property color primary: "#7c5cff"
    property color cyan: "#21d4fd"
    property color green: "#31d0aa"
    property color red: "#ff5d73"
    property color amber: "#ffb84d"

    Material.theme: darkMode ? Material.Dark : Material.Light
    Material.accent: primary
    Material.primary: primary

    function feedbackColor(kind) {
        if (kind === "success") return green
        if (kind === "error") return red
        if (kind === "answer") return cyan
        return text1
    }

    function addEditorWord(w, p, m, e) {
        editorModel.append({"word": w || "", "pos": p || "", "meaning": m || "", "example": e || ""})
    }

    function editorItems() {
        var items = []
        for (var i = 0; i < editorModel.count; ++i) {
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

    FileDialog {
        id: openDialog
        title: "选择单词库文件"
        fileMode: FileDialog.OpenFile
        nameFilters: ["Word book (*.txt)", "All files (*)"]
        onAccepted: appController.loadWordBook(selectedFile)
    }

    FileDialog {
        id: exportDialog
        title: "导出错词本"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text file (*.txt)"]
        onAccepted: appController.exportWrongWords(selectedFile)
    }

    FileDialog {
        id: editorImportDialog
        title: "导入已有词库到编辑器"
        fileMode: FileDialog.OpenFile
        nameFilters: ["Word book (*.txt)", "All files (*)"]
        onAccepted: {
            var words = appController.loadEditorWords(selectedFile)
            if (words.length > 0) {
                editorModel.clear()
                for (var i = 0; i < words.length; ++i) editorModel.append(words[i])
                editorDialog.open()
            }
        }
    }

    FileDialog {
        id: editorSaveDialog
        title: "保存词库"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text file (*.txt)"]
        onAccepted: appController.saveEditorWords(selectedFile, editorItems())
    }

    MessageDialog {
        id: aboutDialog
        title: "关于 VocabMemster"
        text: "VocabMemster 2.0\n\n已重写为 C++ 逻辑层 + QML 表现层。\n支持词库导入、拼写练习、错词本、导出、词库编辑与深色/浅色主题。\n\n词库格式：单词|词性|中文释义|例句"
    }

    MessageDialog {
        id: helpDialog
        title: "使用说明"
        text: "1. 点击“打开词库”导入 .txt 单词文件。\n2. 根据词性、释义和例句输入英文单词。\n3. 回车或点击“提交答案”。答错会自动加入错词本。\n4. Alt+O 打开词库，Alt+S 查看答案，Ctrl+Tab 下一个，Ctrl+Shift+D 切换主题。\n5. 可在“词库编辑器”中新建、导入并保存词库。"
    }

    MessageDialog {
        id: debugDialog
        title: "调试信息"
        text: ""
    }

    Shortcut { sequence: "Alt+O"; onActivated: openDialog.open() }
    Shortcut { sequence: "Alt+S"; onActivated: appController.revealAnswer() }
    Shortcut { sequence: "Ctrl+Tab"; onActivated: appController.nextWord() }
    Shortcut { sequence: "Ctrl+Shift+D"; onActivated: darkMode = !darkMode }

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0; color: bg0 }
            GradientStop { position: 1; color: bg1 }
        }
        Rectangle { anchors.fill: parent; color: "transparent"; border.color: darkMode ? "#223153" : "#d6e3f8"; border.width: 1 }
        Canvas {
            anchors.fill: parent
            opacity: darkMode ? 0.28 : 0.20
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                var g1 = ctx.createRadialGradient(width*0.16, height*0.18, 8, width*0.16, height*0.18, width*0.42)
                g1.addColorStop(0, "#7c5cff")
                g1.addColorStop(1, "transparent")
                ctx.fillStyle = g1; ctx.fillRect(0, 0, width, height)
                var g2 = ctx.createRadialGradient(width*0.86, height*0.20, 8, width*0.86, height*0.20, width*0.34)
                g2.addColorStop(0, "#21d4fd")
                g2.addColorStop(1, "transparent")
                ctx.fillStyle = g2; ctx.fillRect(0, 0, width, height)
                var g3 = ctx.createRadialGradient(width*0.72, height*0.90, 8, width*0.72, height*0.90, width*0.38)
                g3.addColorStop(0, "#31d0aa")
                g3.addColorStop(1, "transparent")
                ctx.fillStyle = g3; ctx.fillRect(0, 0, width, height)
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "文件"
            Action { text: "打开词库..."; shortcut: "Alt+O"; onTriggered: openDialog.open() }
            Action { text: "词库编辑器"; onTriggered: { editorModel.clear(); addEditorWord("", "", "", ""); editorDialog.open() } }
            Action { text: "导入词库到编辑器..."; onTriggered: editorImportDialog.open() }
            MenuSeparator {}
            Action { text: "重置进度"; onTriggered: appController.resetProgress() }
            Action { text: "退出"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "查看"
            Action { text: "错词本"; onTriggered: wrongDrawer.open() }
            Action { text: "导出错词本..."; onTriggered: exportDialog.open() }
            Action { text: "清空错词本"; onTriggered: appController.clearWrongWords() }
            MenuSeparator {}
            Action { text: "调试信息"; onTriggered: { debugDialog.text = appController.debugInfo(); debugDialog.open() } }
        }
        Menu {
            title: "帮助"
            Action { text: "使用说明"; onTriggered: helpDialog.open() }
            Action { text: "关于"; onTriggered: aboutDialog.open() }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 22
        spacing: 20

        Rectangle {
            id: sidebar
            Layout.preferredWidth: 255
            Layout.fillHeight: true
            radius: 28
            color: Qt.rgba(card.r, card.g, card.b, darkMode ? 0.78 : 0.88)
            border.color: line
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 22
                spacing: 18

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    Rectangle {
                        width: 48; height: 48; radius: 16
                        gradient: Gradient {
                            GradientStop { position: 0; color: primary }
                            GradientStop { position: 1; color: cyan }
                        }
                        Label { anchors.centerIn: parent; text: "V"; color: "white"; font.pixelSize: 24; font.bold: true }
                    }
                    ColumnLayout {
                        spacing: 0
                        Label { text: "VocabMemster"; color: text0; font.pixelSize: 20; font.bold: true }
                        Label { text: "Spell • Review • Master"; color: text1; font.pixelSize: 12 }
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: line }

                Button {
                    Layout.fillWidth: true
                    text: "打开词库"
                    icon.name: "document-open"
                    highlighted: true
                    onClicked: openDialog.open()
                }
                Button { Layout.fillWidth: true; text: "词库编辑器"; onClicked: { editorModel.clear(); addEditorWord("", "", "", ""); editorDialog.open() } }
                Button { Layout.fillWidth: true; text: "错词本（" + appController.wrongCount + "）"; onClicked: wrongDrawer.open() }

                Rectangle { Layout.fillWidth: true; height: 1; color: line }

                StatPill { label: "总单词"; value: appController.totalCount; accent: primary }
                StatPill { label: "已掌握"; value: appController.masteredCount; accent: green }
                StatPill { label: "错词"; value: appController.wrongCount; accent: red }
                StatPill { label: "剩余"; value: appController.remainingCount; accent: amber }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: darkMode ? "深色模式" : "浅色模式"; color: text1; Layout.fillWidth: true }
                    Switch { checked: darkMode; onToggled: darkMode = checked }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 18

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 92
                radius: 26
                color: Qt.rgba(card.r, card.g, card.b, darkMode ? 0.70 : 0.85)
                border.color: line

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 18
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        Label { text: appController.loaded ? "今日拼写练习" : "欢迎使用 VocabMemster"; color: text0; font.pixelSize: 28; font.bold: true }
                        Label { text: appController.statusText; color: text1; elide: Text.ElideRight; Layout.fillWidth: true }
                    }
                    ProgressRing {
                        size: 58
                        lineWidth: 7
                        value: appController.progress
                        foreground: appController.completed ? green : primary
                        backgroundColor: darkMode ? "#2a3652" : "#dbe5f5"
                    }
                    Label { text: Math.round(appController.progress * 100) + "%"; color: text0; font.pixelSize: 20; font.bold: true }
                }
            }

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

    Drawer {
        id: wrongDrawer
        width: Math.min(window.width * 0.42, 520)
        height: window.height
        edge: Qt.RightEdge
        modal: true
        background: Rectangle { color: card; border.color: line }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 22
            spacing: 14
            RowLayout {
                Layout.fillWidth: true
                ColumnLayout {
                    Layout.fillWidth: true
                    Label { text: "错词本"; color: text0; font.pixelSize: 26; font.bold: true }
                    Label { text: "共 " + appController.wrongCount + " 个需要复习的单词"; color: text1 }
                }
                ToolButton { text: "✕"; onClicked: wrongDrawer.close() }
            }
            RowLayout {
                Layout.fillWidth: true
                Button { text: "导出"; enabled: appController.wrongCount > 0; onClicked: exportDialog.open() }
                Button { text: "清空"; enabled: appController.wrongCount > 0; onClicked: appController.clearWrongWords() }
            }
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: appController.wrongWordsModel
                spacing: 12
                clip: true
                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 18
                    color: card2
                    border.color: line
                    implicitHeight: wrongCardColumn.implicitHeight + 28
                    ColumnLayout {
                        id: wrongCardColumn
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 8
                        RowLayout {
                            Layout.fillWidth: true
                            Label { text: word; color: text0; font.pixelSize: 20; font.bold: true; Layout.fillWidth: true }
                            Label { text: pos ? "[" + pos + "]" : ""; color: primary; font.bold: true }
                        }
                        Label { text: meaning; color: text0; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                        Label { text: example ? "例句：" + example : ""; color: text1; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                    }
                }
                Label {
                    anchors.centerIn: parent
                    visible: appController.wrongCount === 0
                    text: "暂无错词，保持住！"
                    color: text1
                    font.pixelSize: 18
                }
            }
        }
    }

    Dialog {
        id: editorDialog
        title: "词库编辑器"
        modal: true
        width: Math.min(window.width - 80, 920)
        height: Math.min(window.height - 80, 650)
        standardButtons: Dialog.NoButton
        background: Rectangle { radius: 22; color: card; border.color: line }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 14
            RowLayout {
                Layout.fillWidth: true
                Label { text: "词库编辑器"; color: text0; font.pixelSize: 26; font.bold: true; Layout.fillWidth: true }
                Button { text: "导入"; onClicked: editorImportDialog.open() }
                Button { text: "新增"; onClicked: addEditorWord("", "", "", "") }
                Button { text: "保存"; highlighted: true; onClicked: editorSaveDialog.open() }
                ToolButton { text: "✕"; onClicked: editorDialog.close() }
            }
            Label { text: "格式：单词 | 词性 | 中文释义 | 例句。空单词不会保存。"; color: text1 }
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: editorModel
                spacing: 10
                clip: true
                delegate: Rectangle {
                    width: ListView.view.width
                    radius: 18
                    color: card2
                    border.color: line
                    implicitHeight: 138
                    GridLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        columns: 8
                        columnSpacing: 10
                        rowSpacing: 8
                        TextField { Layout.columnSpan: 2; Layout.fillWidth: true; placeholderText: "单词"; text: word; onTextChanged: editorModel.setProperty(index, "word", text) }
                        TextField { Layout.columnSpan: 1; Layout.fillWidth: true; placeholderText: "词性"; text: pos; onTextChanged: editorModel.setProperty(index, "pos", text) }
                        TextField { Layout.columnSpan: 4; Layout.fillWidth: true; placeholderText: "中文释义"; text: meaning; onTextChanged: editorModel.setProperty(index, "meaning", text) }
                        ToolButton { Layout.columnSpan: 1; text: "删除"; onClicked: editorModel.remove(index) }
                        TextArea {
                            Layout.columnSpan: 8
                            Layout.fillWidth: true
                            Layout.fillHeight: true
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

    Popup {
        id: toast
        property string kind: "info"
        property alias text: toastLabel.text
        x: (window.width - width) / 2
        y: 28
        width: Math.min(520, implicitWidth)
        modal: false
        focus: false
        closePolicy: Popup.NoAutoClose
        padding: 0
        background: Rectangle {
            radius: 18
            color: toast.kind === "error" ? red : (toast.kind === "success" ? green : primary)
            opacity: 0.96
        }
        contentItem: Label {
            id: toastLabel
            color: "white"
            font.pixelSize: 15
            font.bold: true
            padding: 14
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
        Timer { id: toastTimer; interval: 2400; onTriggered: toast.close() }
        onOpened: toastTimer.restart()
    }

    component StatPill: Rectangle {
        property string label
        property int value
        property color accent
        Layout.fillWidth: true
        height: 58
        radius: 18
        color: Qt.rgba(accent.r, accent.g, accent.b, darkMode ? 0.13 : 0.10)
        border.color: Qt.rgba(accent.r, accent.g, accent.b, 0.35)
        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            Rectangle { width: 10; height: 10; radius: 5; color: accent }
            Label { text: label; color: text1; Layout.fillWidth: true }
            Label { text: value; color: text0; font.pixelSize: 22; font.bold: true }
        }
    }

    component EmptyState: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.70, 620)
            spacing: 22
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 130; height: 130; radius: 42
                gradient: Gradient {
                    GradientStop { position: 0; color: primary }
                    GradientStop { position: 1; color: cyan }
                }
                Label { anchors.centerIn: parent; text: "Aa"; color: "white"; font.pixelSize: 44; font.bold: true }
            }
            Label { Layout.fillWidth: true; text: "打开一个单词库，开始高颜值背单词"; color: text0; font.pixelSize: 34; font.bold: true; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
            Label { Layout.fillWidth: true; text: "保留原项目的核心学习逻辑，界面改为现代 Qt Quick / QML。词库每行使用：word|pos|meaning|example。"; color: text1; font.pixelSize: 16; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button { text: "打开词库"; highlighted: true; onClicked: openDialog.open() }
                Button { text: "新建词库"; onClicked: { editorModel.clear(); addEditorWord("example", "n.", "例子", "This is an example."); editorDialog.open() } }
            }
        }
    }

    component StudyPage: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.fill: parent
            spacing: 18

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 34
                color: Qt.rgba(card.r, card.g, card.b, darkMode ? 0.74 : 0.90)
                border.color: line
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 34
                    spacing: 22

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            text: appController.currentPos ? "[ " + appController.currentPos + " ]" : "[ 词性 ]"
                            color: primary
                            font.pixelSize: 18
                            font.bold: true
                            padding: 10
                            background: Rectangle { radius: 14; color: Qt.rgba(primary.r, primary.g, primary.b, 0.13); border.color: Qt.rgba(primary.r, primary.g, primary.b, 0.35) }
                        }
                        Item { Layout.fillWidth: true }
                        Label { text: "尝试次数：" + appController.attemptCount; color: appController.attemptCount > 0 ? amber : text1 }
                    }

                    Item { Layout.preferredHeight: 8 }

                    Label {
                        Layout.fillWidth: true
                        text: appController.currentMeaning || "请加载单词库文件"
                        color: text0
                        font.pixelSize: 42
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Label {
                        Layout.fillWidth: true
                        text: appController.currentExample ? "例句：" + appController.currentExample : ""
                        color: text1
                        font.pixelSize: 17
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Item { Layout.fillHeight: true }

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 24
                        color: Qt.rgba(card2.r, card2.g, card2.b, darkMode ? 0.88 : 0.95)
                        border.color: line
                        implicitHeight: answerArea.implicitHeight + 28
                        ColumnLayout {
                            id: answerArea
                            anchors.fill: parent
                            anchors.margins: 18
                            spacing: 14
                            RowLayout {
                                Layout.fillWidth: true
                                TextField {
                                    id: answerField
                                    Layout.fillWidth: true
                                    enabled: appController.inputEnabled
                                    placeholderText: "输入英文单词，按 Enter 提交"
                                    font.pixelSize: 20
                                    selectByMouse: true
                                    onAccepted: {
                                        appController.submitAnswer(text)
                                        if (appController.inputEnabled) selectAll()
                                    }
                                }
                                Button {
                                    text: "提交答案"
                                    highlighted: true
                                    enabled: appController.inputEnabled
                                    onClicked: {
                                        appController.submitAnswer(answerField.text)
                                        if (appController.inputEnabled) answerField.selectAll()
                                    }
                                }
                            }
                            Label {
                                Layout.fillWidth: true
                                text: appController.feedbackText
                                color: feedbackColor(appController.feedbackKind)
                                font.pixelSize: 20
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                visible: text.length > 0
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                Button { text: "查看答案  Alt+S"; enabled: appController.inputEnabled; onClicked: appController.revealAnswer() }
                Button { text: "下一个  Ctrl+Tab"; highlighted: true; onClicked: { answerField.text = ""; appController.nextWord(); answerField.forceActiveFocus() } }
                Item { Layout.fillWidth: true }
                Button { text: "重置进度"; onClicked: appController.resetProgress() }
            }
        }
    }

    component CompletionPage: Rectangle {
        color: "transparent"
        ColumnLayout {
            anchors.centerIn: parent
            width: Math.min(parent.width * 0.75, 640)
            spacing: 22
            Label { Layout.fillWidth: true; text: "🎉 恭喜你！"; color: text0; font.pixelSize: 46; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            Label { Layout.fillWidth: true; text: "你已经掌握了全部 " + appController.totalCount + " 个单词。错词本中有 " + appController.wrongCount + " 个单词可继续复习。"; color: text1; font.pixelSize: 18; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button { text: "重新开始"; highlighted: true; onClicked: appController.resetProgress() }
                Button { text: "查看错词本"; enabled: appController.wrongCount > 0; onClicked: wrongDrawer.open() }
                Button { text: "导出错词"; enabled: appController.wrongCount > 0; onClicked: exportDialog.open() }
            }
        }
    }

    component ProgressRing: Item {
        property real value: 0
        property int size: 64
        property int lineWidth: 8
        property color foreground: primary
        property color backgroundColor: "#33415f"
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
                ctx.lineWidth = lineWidth
                ctx.lineCap = "round"
                ctx.strokeStyle = backgroundColor
                ctx.beginPath(); ctx.arc(cx, cy, r, 0, Math.PI * 2); ctx.stroke()
                ctx.strokeStyle = foreground
                ctx.beginPath(); ctx.arc(cx, cy, r, -Math.PI / 2, -Math.PI / 2 + Math.PI * 2 * Math.max(0, Math.min(1, value))); ctx.stroke()
            }
        }
    }
}
