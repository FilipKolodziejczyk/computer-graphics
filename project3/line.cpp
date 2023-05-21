#include "line.h"
#include "rectangle.h"

#include "QtCore/qdebug.h"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QString>

Line::Line(QPoint start, QColor color, int width) : Shape(color, width), start(start), end(start) {
}

int Line::snap(const QPoint &point) {
    int distance = getDistance(start, point);
    int distance2 = getDistance(end, point);
    if (distance < distance2)
        std::swap(start, end);
    return std::min(distance, distance2);
}

void Line::draw(QPainter &painter, bool antyaliasing) {
    Shape::draw(painter, antyaliasing);
    painter.setPen(QPen(color, 1));

    if (antyaliasing) {
        drawAntyaliased(painter);
    } else {
        drawStandard(painter);
    }
}

void Line::move(QPoint newEnd) {
    Shape::move(newEnd);
    int dx = newEnd.x() - end.x();
    int dy = newEnd.y() - end.y();
    start.setX(start.x() + dx);
    start.setY(start.y() + dy);
    end = newEnd;
}

void Line::resize(QPoint newEnd) {
    Shape::resize(newEnd);
    end = newEnd;
}

void Line::serialise(QXmlStreamWriter &writer) {
    writer.writeStartElement("line");
    writer.writeAttribute("start", QString::number(start.x()) + "," + QString::number(start.y()));
    writer.writeAttribute("end", QString::number(end.x()) + "," + QString::number(end.y()));
    writer.writeAttribute("color", QString::number(color.red()) + "," + QString::number(color.green()) + "," +
                                   QString::number(color.blue()));
    writer.writeAttribute("width", QString::number(width));
    writer.writeEndElement();
}

Line *Line::deserialise(QXmlStreamReader &reader) {
    QPoint start;
    QPoint end;
    QColor color;
    int width = 0;
    bool antyaliased = false;

    QXmlStreamAttributes attributes = reader.attributes();
    if (attributes.hasAttribute("start")) {
        QStringList startList = attributes.value("start").toString().split(",");
        start.setX(startList[0].toInt());
        start.setY(startList[1].toInt());
    }
    if (attributes.hasAttribute("end")) {
        QStringList endList = attributes.value("end").toString().split(",");
        end.setX(endList[0].toInt());
        end.setY(endList[1].toInt());
    }
    if (attributes.hasAttribute("color")) {
        QStringList colorList = attributes.value("color").toString().split(",");
        color.setRed(colorList[0].toInt());
        color.setGreen(colorList[1].toInt());
        color.setBlue(colorList[2].toInt());
    }
    if (attributes.hasAttribute("width")) {
        width = attributes.value("width").toInt();
    }
    if (attributes.hasAttribute("antyaliased")) {
        antyaliased = attributes.value("antyaliased").toInt();
    }

    auto *line = new Line(start, color, width);
    line->end = end;
    return line;
}

void Line::drawStandard(QPainter &painter) {
    QPoint start = this->start;
    QPoint end = this->end;
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
    QPoint start = this->start;
    QPoint end = this->end;

    bool reversed = std::abs(end.y() - start.y()) > std::abs(end.x() - start.x());
    if (reversed) {
        std::swap(start.rx(), start.ry());
        std::swap(end.rx(), end.ry());
    }

    if (start.x() > end.x())
        std::swap(start, end);

    double d = 1.0 * (end.y() - start.y()) / (end.x() - start.x());

    int x = std::round(start.x());
    double temp;
    double yFraction = modf(start.y() + d * (x - start.x()), &temp);
    auto yInt = static_cast<int>(temp);

    double xi = 1 - modf(start.x() + 0.5, &temp);

    if (reversed) {
        color.setAlphaF((1 - yFraction) * xi);
        painter.setPen(QPen(color));
        brush(painter, yInt, x);
        color.setAlphaF(yFraction * xi);
        painter.setPen(QPen(color));
        brush(painter, yInt + 1, x);
    } else {
        color.setAlphaF((1 - yFraction) * xi);
        painter.setPen(QPen(color));
        brush(painter, x, yInt);
        color.setAlphaF(yFraction * xi);
        painter.setPen(QPen(color));
        brush(painter, x, yInt + 1);
    }

    double q = start.y() + d * (x - start.x()) + d;
    double n = modf(q, &temp);
    // Strange error, using temp instead of std::floor(q) beaks it

    for (int a = std::round(start.x()) + 1; a <= std::round(end.x()) - 1; a++) {
        if (reversed) {
            color.setAlphaF(1 - n);
            painter.setPen(QPen(color));
            brush(painter, std::floor(q), a);
            color.setAlphaF(n);
            painter.setPen(QPen(color));
            brush(painter, std::floor(q) + 1, a);
        } else {
            color.setAlphaF(1 - n);
            painter.setPen(QPen(color));
            brush(painter, a, std::floor(q));
            color.setAlphaF(n);
            painter.setPen(QPen(color));
            brush(painter, a, std::floor(q) + 1);
        }
        q += d;
    }

    color.setAlphaF(1);
}

QList<Shape *> Line::LiangBarskyClip(const Rectangle *clipper) const {
    float dx = end.x() - start.x();
    float dy = end.y() - start.y();

    float tE = 0, tL = 1;
    if (Clip(-dx, start.x() - clipper->left(), tE, tL))
        if (Clip(dx, clipper->right() - start.x(), tE, tL))
            if (Clip(-dy, start.y() - clipper->top(), tE, tL))
                if (Clip(dy, clipper->bottom() - start.y(), tE, tL)) {
                    auto *line = new Line(QPoint(start.x(), start.y()), QColor(Qt::red), width);
                    line->resize(end);
                    if (tL < 1) {
                        line->end.setX(start.x() + dx * tL);
                        line->end.setY(start.y() + dy * tL);
                    }
                    if (tE > 0) {
                        line->start.setX(start.x() + dx * tE);
                        line->start.setY(start.y() + dy * tE);
                    }
                    return QList<Shape *>{line};
                }
    return QList<Shape *>{};
}

bool Line::Clip(float denom, float numer, float& tE, float& tL) {
    if (denom == 0) { // parallel
        if (numer < 0) // outside - discard
            return false;
        return true; //skip to next edge
    }

    float t = numer / denom;
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