import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import Qt.labs.settings 1.0

import OpenHD 1.0

import "../elements"

BaseWidget {
    id: flightModeWidget
    width: 212
    height: 48

    visible: settings.show_flight_mode

    widgetIdentifier: "flight_mode_widget"

    defaultAlignment: 3
    defaultHCenter: true
    defaultVCenter: false

    hasWidgetDetail: true
    hasWidgetAction: true //--TURN TO TRUE TO SEE THE FLIGHT MODE ACTIONS
    widgetDetailComponent: ScrollView {

        //contentHeight: horizonSettingsColumn.height
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        clip: true

        ColumnLayout {
            id: flightModeSettings
            Item {
                width: 230
                height: 42
                Text {
                    id: flightModeSettingsTitle
                    text: qsTr("FLIGHT MODE")
                    color: "white"
                    height: parent.height - 10
                    width: parent.width
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: detailPanelFontPixels
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    id: flightModeSettingsTitleUL
                    y: 34
                    width: parent.width
                    height: 3
                    color: "white"
                    radius: 5
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    id: opacityTitle
                    text: qsTr("Transparency")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Slider {
                    id: flight_mode_opacity_Slider
                    orientation: Qt.Horizontal
                    from: .1
                    value: settings.flight_mode_opacity
                    to: 1
                    stepSize: .1
                    height: parent.height
                    anchors.rightMargin: 0
                    anchors.right: parent.right
                    width: parent.width - 96

                    onValueChanged: {
                        settings.flight_mode_opacity = flight_mode_opacity_Slider.value
                    }
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    text: qsTr("Size")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Slider {
                    id: flight_mode_size_Slider
                    orientation: Qt.Horizontal
                    from: .5
                    value: settings.flight_mode_size
                    to: 3
                    stepSize: .1
                    height: parent.height
                    anchors.rightMargin: 0
                    anchors.right: parent.right
                    width: parent.width - 96

                    onValueChanged: {
                        settings.flight_mode_size = flight_mode_size_Slider.value
                    }
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    text: qsTr("Lock to Horizontal Center")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    width: 32
                    height: parent.height
                    anchors.rightMargin: 6
                    anchors.right: parent.right
                    checked: {
                        // @disable-check M222
                        var _hCenter = settings.value(hCenterIdentifier,
                                                      defaultHCenter)
                        // @disable-check M223
                        if (_hCenter === "true" || _hCenter === 1
                                || _hCenter === true) {
                            checked = true
                            // @disable-check M223
                        } else {
                            checked = false
                        }
                    }

                    onCheckedChanged: settings.setValue(hCenterIdentifier,
                                                        checked)
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    text: qsTr("Lock to Vertical Center")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    width: 32
                    height: parent.height
                    anchors.rightMargin: 6
                    anchors.right: parent.right
                    checked: {
                        // @disable-check M222
                        var _vCenter = settings.value(vCenterIdentifier,
                                                      defaultVCenter)
                        // @disable-check M223
                        if (_vCenter === "true" || _vCenter === 1
                                || _vCenter === true) {
                            checked = true
                            // @disable-check M223
                        } else {
                            checked = false
                        }
                    }

                    onCheckedChanged: settings.setValue(vCenterIdentifier,
                                                        checked)
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    text: qsTr("Lock to Horizontal Center")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    width: 32
                    height: parent.height
                    anchors.rightMargin: 6
                    anchors.right: parent.right
                    checked: {
                        // @disable-check M222
                        var _hCenter = settings.value(hCenterIdentifier,
                                                      defaultHCenter)
                        // @disable-check M223
                        if (_hCenter === "true" || _hCenter === 1
                                || _hCenter === true) {
                            checked = true
                            // @disable-check M223
                        } else {
                            checked = false
                        }
                    }

                    onCheckedChanged: settings.setValue(hCenterIdentifier,
                                                        checked)
                }
            }
            Item {
                width: 230
                height: 32
                Text {
                    text: qsTr("Lock to Vertical Center")
                    color: "white"
                    height: parent.height
                    font.bold: true
                    font.pixelSize: detailPanelFontPixels
                    anchors.left: parent.left
                    verticalAlignment: Text.AlignVCenter
                }
                Switch {
                    width: 32
                    height: parent.height
                    anchors.rightMargin: 6
                    anchors.right: parent.right
                    checked: {
                        // @disable-check M222
                        var _vCenter = settings.value(vCenterIdentifier,
                                                      defaultVCenter)
                        // @disable-check M223
                        if (_vCenter === "true" || _vCenter === 1
                                || _vCenter === true) {
                            checked = true
                            // @disable-check M223
                        } else {
                            checked = false
                        }
                    }

                    onCheckedChanged: settings.setValue(vCenterIdentifier,
                                                        checked)
                }
            }
        }
    }

    //---------------------------ACTION WIDGET COMPONENT BELOW-----------------------------
    widgetActionComponent: ScrollView {

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        clip: true


        /*
        PLANE_MODE_MANUAL=0,
        PLANE_MODE_CIRCLE=1,
        PLANE_MODE_STABILIZE=2,
        PLANE_MODE_TRAINING=3,
        PLANE_MODE_ACRO=4,
        PLANE_MODE_FLY_BY_WIRE_A=5,
        PLANE_MODE_FLY_BY_WIRE_B=6,
        PLANE_MODE_CRUISE=7,
        PLANE_MODE_AUTOTUNE=8,
        PLANE_MODE_AUTO=10,
        PLANE_MODE_RTL=11,
        PLANE_MODE_LOITER=12,
        PLANE_MODE_TAKEOFF=13

VTOL
00017     17 : 'QSTABILIZE',
00018     18 : 'QHOVER',
00019     19 : 'QLOITER',
00020     20 : 'QLAND',
00021     21 : 'QRTL',

       COPTER_MODE_STABILIZE=0
       COPTER_MODE_ACRO=1
       COPTER_MODE_ALT_HOLD=2
       COPTER_MODE_AUTO=3
       COPTER_MODE_GUIDED=4
       COPTER_MODE_LOITER=5
       COPTER_MODE_RTL=6
       COPTER_MODE_CIRCLE=7
       COPTER_MODE_LAND=9
       COPTER_MODE_DRIFT=11
       COPTER_MODE_SPORT=13
       COPTER_MODE_FLIP=14
       COPTER_MODE_AUTOTUNE=15
       COPTER_MODE_POSHOLD=16
       COPTER_MODE_BRAKE=17
       COPTER_MODE_THROW=18
       COPTER_MODE_AVOID_ADSB=19
       COPTER_MODE_GUIDED_NOGPS=20
       COPTER_MODE_SMART_RTL=21
*/
        Column {
            width: 200
            spacing: 10

            Text {
                height: 32
                text: {
                    return qsTr("Only For Ardupilot/PX4");
                }
                color: "white"
                font.bold: true
                font.pixelSize: detailPanelFontPixels
                anchors.left: parent.left
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("RTL")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 6;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 11;
                    }
                }
                onCheckedChanged: {
                    if (checked == true) {
                        //_fcMavlinkSystem.set_Requested_Flight_Mode(msg_id);
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                        console.log("FLIGHT MODE MSD ID=");
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("STABILIZE")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 0;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 2;
                    }
                }
                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("LOITER")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 5;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 12;
                    }
                }

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("CIRCLE")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 7;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 1;
                    }
                }
                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("AUTO")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 3;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 10;
                    }
                }

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.supports_basic_commands

                text_off: qsTr("AUTOTUNE")
                msg_id: {
                    if (_fcMavlinkSystem.mav_type == "ARDUCOPTER"){
                        return 15;
                    }
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return 8;
                    }
                }

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

  //-----------------------DIFFERNT from plane to copter to vtol

            ConfirmSlider {
                visible: {
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return true;
                    } else {
                        return false;
                    }
                }
                text_off: qsTr("MANUAL")
                msg_id: 0

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: {
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return true;
                    } else {
                        return false;
                    }
                }
                text_off: qsTr("FBWA")
                msg_id: 5

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: {
                    if (_fcMavlinkSystem.mav_type == "ARDUPLANE" || "VTOL"){
                        return true;
                    } else {
                        return false;
                    }
                }

                text_off: qsTr("FBWB")
                msg_id: 6

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "VTOL"

                text_off: qsTr("QSTABILIZE")
                msg_id: 17

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "VTOL"

                text_off: qsTr("QHOVER")
                msg_id: 18

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "VTOL"

                text_off: qsTr("QLOITER")
                msg_id: 19

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "VTOL"

                text_off: qsTr("QLAND")
                msg_id: 20

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "VTOL"

                text_off: qsTr("QRTL")
                msg_id: 21

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }

            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "ARDUCOPTER"

                text_off: qsTr("ALT_HOLD")
                msg_id: 2

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }
            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "ARDUCOPTER"

                text_off: qsTr("POSHOLD")
                msg_id: 16

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }
            ConfirmSlider {
                visible: _fcMavlinkSystem.mav_type == "ARDUCOPTER"

                text_off: qsTr("ACRO")
                msg_id: 1

                onCheckedChanged: {
                    if (checked == true) {
                        _fcMavlinkSystem.flight_mode_cmd(msg_id);
                    }
                }
            }
        }
    }

    Item {
        id: widgetInner
        anchors.fill: parent
        scale: settings.flight_mode_size

        Text {
            id: flight_mode_icon
            width: 24
            height: 48
            color: settings.color_shape
            opacity: settings.flight_mode_opacity
            text: "\uf072"
            anchors.right: flight_mode_text.left
            anchors.rightMargin: 6
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Font Awesome 5 Free"
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20
            style: Text.Outline
            styleColor: settings.color_glow
        }
        Text {
            id: flight_mode_text
            height: 48
            color: settings.color_text
            opacity: settings.flight_mode_opacity
            text: _fcMavlinkSystem.flight_mode
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 24
            font.family: settings.font_text
            elide: Text.ElideRight
            style: Text.Outline
            styleColor: settings.color_glow
        }
        Text {
            id:left_bracket
            height: 48
            visible: _fcMavlinkSystem.armed
            color: "red"
            text: "["
            opacity: settings.flight_mode_opacity
            anchors.left: parent.left
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 24
            font.family: settings.font_text
            elide: Text.ElideRight
            style: Text.Outline
            styleColor: settings.color_glow
        }
        Text {
            id:right_bracket
            height: 48
            visible: _fcMavlinkSystem.armed
            color: "red"
            text: "]"
            opacity: settings.flight_mode_opacity
            anchors.right: parent.right
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 24
            font.family: settings.font_text
            elide: Text.ElideRight
            style: Text.Outline
            styleColor: settings.color_glow
        }
    }
}
