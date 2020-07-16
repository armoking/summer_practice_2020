#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <bits/stdc++.h>

#include "common.h"
#include "filter.h"

using namespace std;

const QString PATH_TO_SOURCE = "C:\\Users\\User\\Desktop\\photo2.jpg";
const QString PATH_TO_RESULT = "C:\\Users\\User\\Desktop\\photo_result.jpg";

int main() {
    Image image = QImage(PATH_TO_SOURCE);
    Filter(image).SaveImage(PATH_TO_RESULT);
}
