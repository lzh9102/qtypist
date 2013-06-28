/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: http://code.google.com/p/qtypist
 */

#ifndef PATHS_H
#define PATHS_H

#include <QString>

class Paths
{
public:
    static void setAppPath(const QString& path);
    static QString appPath();

    static QString dataPath();
    static QString dataFileName(const QString& filename);
    static QString translationPath();
    static QString audioCachePath();
    static QString mplayerExecutable();
};

#endif // PATHS_H
