// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/multi_head_manager.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>

#include "ui/widgets/wallpaper_item.h"
#include "ui/utils/xrandr_wrapper.h"

namespace installer {

MultiHeadManager::MultiHeadManager(QObject* parent)
    : QObject(parent),
      wallpaper_items_() {
  this->setObjectName(QStringLiteral("wallpaper_manager"));

  desktop_widget_ = QApplication::desktop();
  this->initConnections();
}

void MultiHeadManager::updateWallpaper() {
  qDebug() << "MultiHeadManager() updateWallpaper";
  // Clear old wallpaper items
  for (WallpaperItem* item : wallpaper_items_) {
    delete item;
    item = nullptr;
  }
  wallpaper_items_.clear();

  XRandR xrandr;
  QString out;
  RunXRandR(out);

  if (ParseXRandR(out, xrandr)) {
    for (const Output& output : xrandr.outputs) {
      if (output.is_connected) {
        const QRect geometry(output.x, output.y, output.width, output.height);
        WallpaperItem* item = new WallpaperItem(geometry);
        wallpaper_items_.append(item);
        item->show();
      }
    }
  } else {
    qCritical() << "updateWallpaper() parse xrandr failed!";
  }
}

void MultiHeadManager::switchXRandRMode() {
  SwitchModeWrapper();
}

void MultiHeadManager::initConnections() {
  connect(desktop_widget_, &QDesktopWidget::primaryScreenChanged,
          this, &MultiHeadManager::onPrimaryScreenChanged);
  connect(desktop_widget_, &QDesktopWidget::screenCountChanged,
          this, &MultiHeadManager::onScreenCountChanged);
}

void MultiHeadManager::onPrimaryScreenChanged() {
  this->updateWallpaper();
  XRandR xrandr;
  QString out;
  RunXRandR(out);
  if (ParseXRandR(out, xrandr)) {
    for (const Output& output : xrandr.outputs) {
      if (output.is_connected && output.is_primary) {
        const QRect geometry(output.x, output.y, output.width, output.height);
        emit this->primaryScreenChanged(geometry);
        break;
      }
    }
  } else {
    qCritical() << "multi head manager parse xrandr failed:";
  }
}

void MultiHeadManager::onScreenCountChanged() {
  this->updateWallpaper();
}

}  // namespace installer