// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_FS_MODEL_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_FS_MODEL_H

#include <QAbstractListModel>

#include "partman/fs.h"

namespace ui {

class PartitionDelegate;

class FsModel : public QAbstractListModel {
  Q_OBJECT

 public:
  FsModel(PartitionDelegate* delegate, QObject* parent = nullptr);

  // Update fs list.
  void updateList();

  // Index fs type.
  // Returns -1 if not found.
  int index(partman::FsType fs_type) const;

  virtual int rowCount(const QModelIndex& parent) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

 private:
  PartitionDelegate* delegate_ = nullptr;

  partman::FsTypeList fs_list_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_FS_MODEL_H