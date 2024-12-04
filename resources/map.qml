import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    id: window

    property double oldLat: 21.012915
    property double oldLng: 105.85667
    property Component comMarker: mapMarker
    property Component comRoute: routeLine

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldLat, oldLng)
        zoomLevel: 14 

        onMapItemsChanged: {
            if (mapView.mapItems.length > 0) {
                zoomTimer.restart()
            }
        }
    }

    Timer {
        id: zoomTimer
        interval: 100  
        onTriggered: {
            mapView.fitViewportToMapItems()
        }
    }

    function setCenter(lat, lng) {
        mapView.pan(oldLat - lat, oldLng - lng)
        oldLat = lat
        oldLng = lng
    }

    function addMarker(lat, lng) {
        var item = comMarker.createObject(window, {
                                           coordinate: QtPositioning.coordinate(lat, lng)
                                          })
        mapView.addMapItem(item)
    }

    function drawRoute(coordinates) {
        mapView.clearMapItems()

        var routeCoords = []
        for (var i = 0; i < coordinates.length; i += 2) {
            var coord = QtPositioning.coordinate(coordinates[i], coordinates[i+1])
            routeCoords.push(coord)
            
            addMarker(coordinates[i], coordinates[i+1])
        }

        var routeItem = comRoute.createObject(window, {
            path: routeCoords
        })
        mapView.addMapItem(routeItem)
    }

    Component {
        id: mapMarker
        MapQuickItem {
            id: markerImg
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height

            sourceItem: Image {
                id: image
                source: "qrc:/images/marker.png"
                width: 30
                height: 30
            }
        }
    }

    Component {
        id: routeLine
        MapPolyline {
            line.color: "blue"
            line.width: 4
        }
    }
}