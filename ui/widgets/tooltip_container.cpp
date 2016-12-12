// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/tooltip_container.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

namespace installer {

namespace {

const int kBorderRadius = 5;
const int kBorderDiameter = kBorderRadius * 2;

// _____
// \ | /
//  \|/
const int kTriangleHeight = 6;

}  // namespace

TooltipContainer::TooltipContainer(QWidget* parent) : QFrame(parent) {
  this->setObjectName("tooltip_container");
}

void TooltipContainer::setWidget(QWidget* widget) {
  child_widget_ = widget;
  child_widget_->setParent(this);
}

QWidget* TooltipContainer::widget() const {
  return child_widget_;
}

void TooltipContainer::popup(const QPoint& pos) {
  const QSize size = child_widget_->size();
  this->resize(size.width(), size.height() + kTriangleHeight);
  this->move(pos.x() - size.width() / 2, pos.y() - size.height());
  this->show();
}

void TooltipContainer::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  qDebug() << "draw container";

  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing,
                         true);
  const int kWidth = this->width();
  const int kHalfWidth = kWidth / 2;
  const int kHeight = this->height();

  // TODO(xushaohua):
//  if (true) {
//    return;
//  }

  // First draw background with round corner.
  QPainterPath background_path;
  background_path.moveTo(kWidth, kBorderRadius);
  background_path.arcTo(kWidth - kBorderDiameter, 0,
                        kBorderDiameter, kBorderDiameter,
                        0.0, 90.0);
  background_path.lineTo(kBorderRadius, 0);
  background_path.arcTo(0, 0, kBorderDiameter, kBorderDiameter, 90.0, 90.0);
  background_path.lineTo(0, kHeight - kBorderRadius - kTriangleHeight);
  background_path.arcTo(0, kHeight - kBorderDiameter - kTriangleHeight,
                        kBorderDiameter, kBorderDiameter,
                        180.0, 90.0);
  background_path.lineTo(kHalfWidth - kTriangleHeight,
                         kHeight - kTriangleHeight);
  // Draw isosceles right-angled triangle in bottom center of label.
  background_path.lineTo(kHalfWidth, kHeight);
  background_path.lineTo(kHalfWidth + kTriangleHeight,
                         kHeight - kTriangleHeight);
  background_path.lineTo(kWidth - kBorderRadius, kHeight - kTriangleHeight);
  background_path.arcTo(kWidth - kBorderDiameter,
                        kHeight - kBorderDiameter - kTriangleHeight,
                        kBorderDiameter, kBorderDiameter,
                        270.0, 90.0);
  background_path.closeSubpath();
  painter.fillPath(background_path, QBrush(Qt::white));
}

}  // namespace installer