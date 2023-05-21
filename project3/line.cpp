#include "line.h"
#include "rectangle.h"

#include "QtCore/qdebug.h"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QString>

Line::Line(QPoint start, QColor color, int width) : Shape(color, width), _start(start), _end(start) {
}

int Line::snap(const QPoint &point) {
    int distance = qRound(getDistance(_start, point));
    int distance2 = qRound(getDistance(_end, point));
    if (distance < distance2)
        std::swap(_start, _end);
    return std::min(distance, distance2);
}

void Line::draw(QPainter &painter, bool antyaliasing) {
    painter.setPen(QPen(_color, 1));

    if (antyaliasing) {
        drawAntyaliased(painter);
    } else {
        drawStandard(painter);
    }
}

void Line::move(QPoint newEnd) {
    int dx = newEnd.x() - _end.x();
    int dy = newEnd.y() - _end.y();
    _start.setX(_start.x() + dx);
    _start.setY(_start.y() + dy);
    _end = newEnd;
}

void Line::resize(QPoint newEnd) {
    _end = newEnd;
}

void Line::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("line");
    writer.writeAttribute("_start", QString::number(_start.x()) + "," + QString::number(_start.y()));
    writer.writeAttribute("_end", QString::number(_end.x()) + "," + QString::number(_end.y()));
    writer.writeAttribute("_color", QString::number(_color.red()) + "," + QString::number(_color.green()) + "," +
                                    QString::number(_color.blue()));
    writer.writeAttribute("_width", QString::number(_width));
    writer.writeEndElement();
}

Line *Line::deserialise(QXmlStreamReader &reader) {
    QPoint start;
    QPoint end;
    QColor color;
    int width = 0;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("_start")) {
        QStringList startList = attributes.value("_start").toString().split(",");
        start.setX(startList[0].toInt());
        start.setY(startList[1].toInt());
    }
    if (attributes.hasAttribute("_end")) {
        QStringList endList = attributes.value("_end").toString().split(",");
        end.setX(endList[0].toInt());
        end.setY(endList[1].toInt());
    }
    if (attributes.hasAttribute("_color")) {
        QStringList colorList = attributes.value("_color").toString().split(",");
        color.setRed(colorList[0].toInt());
        color.setGreen(colorList[1].toInt());
        color.setBlue(colorList[2].toInt());
    }
    if (attributes.hasAttribute("_width")) {
        width = attributes.value("_width").toInt();
    }

    auto *line = new Line(start, color, width);
    line->_end = end;
    return line;
}

void Line::drawStandard(QPainter &painter) {
    QPoint start = this->_start;
    QPoint end = this->_end;
    if (start.x() > end.x())
        std::swap(start, end);

    if (abs(start.y() - end.y()) < abs(start.x() - end.x())) {
        if (start.x() > end.x())
            std::swap(start, end);

        int dx = end.x() - start.x();
        int dy = end.y() - start.y();
        int yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int d = 2 * dy - dx;
        int y = start.y();

        for (int x = start.x(); x <= end.x(); x++) {
            brush(painter, x, y);
            if (d > 0) {
                y += yi;
                d -= 2 * dx;
            }
            d += 2 * dy;
        }
    } else {
        if (start.y() > end.y())
            std::swap(start, end);

        int dx = end.x() - start.x();
        int dy = end.y() - start.y();
        int xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int d = 2 * dx - dy;
        int x = start.x();

        for (int y = start.y(); y <= end.y(); y++) {
            brush(painter, x, y);
            if (d > 0) {
                x += xi;
                d -= 2 * dy;
            }
            d += 2 * dx;
        }
    }

}

void Line::drawAntyaliased(QPainter &painter) {
    QPoint start = this->_start;
    QPoint end = this->_end;

    bool reversed = std::abs(end.y() - start.y()) > std::abs(end.x() - start.x());
    if (reversed) {
        std::swap(start.rx(), start.ry());
        std::swap(end.rx(), end.ry());
    }

    if (start.x() > end.x())
        std::swap(start, end);

    double d = 1.0 * (end.y() - start.y()) / (end.x() - start.x());

    int x = start.x();
    double temp;
    double yFraction = modf(start.y() + d * (x - start.x()), &temp);
    auto yInt = static_cast<int>(temp);

    double xi = 1 - modf(start.x() + 0.5, &temp);

    if (reversed) {
        _color.setAlphaF(static_cast<float>((1 - yFraction) * xi));
        painter.setPen(QPen(_color));
        brush(painter, yInt, x);
        _color.setAlphaF(static_cast<float>((yFraction) * xi));
        painter.setPen(QPen(_color));
        brush(painter, yInt + 1, x);
    } else {
        _color.setAlphaF(static_cast<float>((1 - yFraction) * xi));
        painter.setPen(QPen(_color));
        brush(painter, x, yInt);
        _color.setAlphaF(static_cast<float>((yFraction) * xi));
        painter.setPen(QPen(_color));
        brush(painter, x, yInt + 1);
    }

    double q = start.y() + d * (x - start.x()) + d;
    double n = modf(q, &temp);
    // Strange error, using temp instead of std::floor(q) beaks it

    for (int a = start.x() + 1; a <= end.x() - 1; a++) {
        if (reversed) {
            _color.setAlphaF(1 - static_cast<float>(n));
            painter.setPen(QPen(_color));
            brush(painter, std::floor(q), a);
            _color.setAlphaF(static_cast<float>(n));
            painter.setPen(QPen(_color));
            brush(painter, qFloor(q) + 1, a);
        } else {
            _color.setAlphaF(1 - static_cast<float>(n));
            painter.setPen(QPen(_color));
            brush(painter, a, std::floor(q));
            _color.setAlphaF(static_cast<float>(n));
            painter.setPen(QPen(_color));
            brush(painter, a, qFloor(q) + 1);
        }
        q += d;
    }

    _color.setAlphaF(1);
}

QList<Shape *> Line::LiangBarskyClip(const Rectangle *clipper) const {
    double dx = _end.x() - _start.x();
    double dy = _end.y() - _start.y();
    double tE = 0, tL = 1;

    if (Clip(-dx, _start.x() - clipper->left(), tE, tL))
        if (Clip(dx, clipper->right() - _start.x(), tE, tL))
            if (Clip(-dy, _start.y() - clipper->top(), tE, tL))
                if (Clip(dy, clipper->bottom() - _start.y(), tE, tL)) {
                    auto *line = new Line(QPoint(_start.x(), _start.y()), QColor(Qt::red), _width);
                    line->resize(_end);
                    if (tL < 1) {
                        line->_end.setX(qRound(_start.x() + dx * tL));
                        line->_end.setY(qRound(_start.y() + dy * tL));
                    }
                    if (tE > 0) {
                        line->_start.setX(qRound(_start.x() + dx * tE));
                        line->_start.setY(qRound(_start.y() + dy * tE));
                    }
                    return QList<Shape *>{line};
                }
    return QList<Shape *>{};
}

bool Line::Clip(double denom, double numer, double &tE, double &tL) {
    if (denom == 0) { // parallel
        if (numer < 0) // outside - discard
            return false;
        return true; //skip to next edge
    }

    double t = numer / denom;
    if (denom < 0) { // potentially leaving
        if (t > tL)
            return false; // outside - discard
        if (t > tE)
            tE = t;
    } else {
        if (t < tE)
            return false; // outside - discard
        if (t < tL)
            tL = t;
    }
    return true;
}