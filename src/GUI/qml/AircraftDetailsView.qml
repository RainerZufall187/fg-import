import QtQuick 2.0
import FlightGear.Launcher 1.0
import "."

Rectangle {
    id: root
    property alias aircraftURI: aircraft.uri

    color: "white"

    MouseArea {
        // consume all mouse-clicks on the detail view
        anchors.fill: parent
    }

    AircraftInfo
    {
        id: aircraft
    }

    Column {
        width: root.width - (Style.margin * 2)
        spacing: Style.margin
        anchors.horizontalCenter: parent.horizontalCenter

        AircraftVariantChoice {
            id: headingBox
            fontPixelSize: 30
            popupFontPixelSize: 18

            anchors {
                margins: Style.strutSize * 2 // space for back button
                left: parent.left
                right: parent.right
            }

            aircraft: aircraftURI
            currentIndex: aircraft.variant

            onSelected: {
                aircraft.variant = index
                _launcher.selectedAircraft = aircraft.uri;
            }
        }

        // this element normally hides itself unless needed
        AircraftWarningPanel {
            id: warningBox
            aircraftStatus: aircraft.status
            requiredFGVersion: aircraft.minimumFGVersion
            width: parent.width
        }

        // thumbnails + description + authors container
        Item {
            width: parent.width
            height: childrenRect.height

            Rectangle {
                id: thumbnailBox
                // thumbnail border

                border.width: 1
                border.color: "#7f7f7f"

                width: thumbnail.width
                height: thumbnail.height

                ThumbnailImage {
                    id: thumbnail

                    aircraftUri: root.aircraftURI
                    maximumSize.width: 172
                    maximumSize.height: 128
                }
            }

            Column {
                anchors.left: thumbnailBox.right
                anchors.leftMargin: Style.margin
                anchors.right: parent.right
                spacing: Style.margin

                Text {
                    id: aircraftDescription
                    text: aircraft.description
                    width: parent.width
                    wrapMode: Text.WordWrap
                    visible: aircraft.description != ""
                    font.pixelSize: 14
                }

                Text {
                    id: aircraftAuthors
                    text: qsTr("by %1").arg(aircraft.authors)
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    visible: (aircraft.authors != undefined)

                }
            }

        }

        AircraftDownloadPanel {
            visible: aircraft.isPackaged
            width: parent.width
            uri: aircraft.uri
            installStatus: aircraft.installStatus
            packageSize: aircraft.packageSize
            downloadedBytes: aircraft.downloadedBytes
        }

        AircraftPreviewPanel {
            id: previews
            width: parent.width
            previews: aircraft.previews
            visible: aircraft.previews.length > 0
        }

        Grid {
            id: ratingGrid
            anchors.left: parent.left

            visible: aircraft.ratings != undefined

            rows: 2
            columns: 3
            rowSpacing: Style.margin
            columnSpacing: Style.margin

            Text {
                id: ratingsLabel
                text: qsTr("Ratings:")
            }


            AircraftRating {
                title: qsTr("Flight model")
                Binding on value {
                    when: aircraft.ratings != undefined
                    value: aircraft.ratings[0]
                }
            }

            AircraftRating {
                title: qsTr("Systems")
                Binding on value {
                    when: aircraft.ratings != undefined
                    value: aircraft.ratings[1]
                }
            }

            Item {
                width: ratingsLabel.width
                height: 1
            } // placeholder

            AircraftRating {
                title: qsTr("Cockpit")
                Binding on value {
                    when: aircraft.ratings != undefined
                    value: aircraft.ratings[2]
                }
            }

            AircraftRating {
                title: qsTr("Exterior")
                Binding on value {
                    when: aircraft.ratings != undefined
                    value: aircraft.ratings[3]
                }
            }
        }

        Text {
            text: qsTr("Local file location: %1").arg(aircraft.pathOnDisk);
            visible: aircraft.pathOnDisk != undefined
        }

    } // main layout column
}