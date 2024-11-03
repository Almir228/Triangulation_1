#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


class Point3D {
public:
    double x, y, z;

    // Конструктор
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Point3D operator+(const Point3D& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    const Point3D operator-(const Point3D& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
    const Point3D operator*(const double& a) const{
        return {a*x, a*y, a*z};
    }


    Point3D vector_tweak(Point3D vector, double delta , bool inplace = true) {
        if (inplace) {
            x+=delta*vector.x;
            y+=delta*vector.y;
            z+=delta*vector.z;
            return *this;
        } else {
            // Создаем новую точку с измененными координатами
            return {x + delta*vector.x, y + delta*vector.y, z + delta*vector.z};
        }
    }
    // Метод для вывода координат
    void print() const {
        std::cout << "Point(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
    //метод для вычисления векторного произведения
    Point3D cross(const Point3D& other) const {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }

    // Метод для вычисления расстояния до другой точки
    double distanceTo(const Point3D& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
    }
    //метод для деления точки на число
    Point3D operator/(const double& a) const{
        return {x/a, y/a, z/a};
    }
};


//Функция для вычисления детерминаента 3 мерного вектора
double determinant(const Point3D& a, const Point3D& b, const Point3D& c) {
    return a.x * (b.y * c.z - b.z * c.y) - a.y * (b.x * c.z - b.z * c.x) + a.z * (b.x * c.y - b.y * c.x);
}

//Функция для вычисления скалярного произведения 3 мерного вектора
double dotProduct(const Point3D& a, const Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//Функция для вычисления угла между векторами 3 мерного вектора
double angleBetweenVectors(const Point3D& a, const Point3D& b) {
    return std::acos(dotProduct(a, b) / (a.distanceTo({0, 0, 0}) * b.distanceTo({0, 0, 0})));
}

//Функция для вычисления вектора единичной нормали к двум векторам
Point3D normalVector(const Point3D& a, const Point3D& b) {
    return a.cross(b)/a.cross(b).distanceTo({0, 0, 0});
}

//Функция для вычисления знака поворота от одного вектора к другому вокруг единичной нормали
int sign(const Point3D& a, const Point3D& b) {
    return determinant(a, b, normalVector(a,b)) > 0 ? 1 : -1;
}



class Triangle {
public:
    Point3D p1;
    Point3D p2;
    Point3D p3;
    // Конструктор принимает ссылки на объекты Point3D
    Triangle(const Point3D& a, const Point3D& b, const Point3D& c) : p1(a), p2(b), p3(c) {}



    // Метод для проверки, находится ли точка p внутри треугольника
    bool contains(const Point3D& p) const {
        // Векторы от вершин треугольника к точке p
        Point3D v0 = p1 - p;
        Point3D v1 = p2 - p;
        Point3D v2 = p3 - p;

        // Условие на принадлежность плоскости треугольника
        if (determinant(v0, v1, v2) < 0) {
            return false;
        }
        //проверяем на то что точка находится внутри треугольника по условию того что повороты от v0 к v1 и v1 к v2 и v2 к v0 должны быть одинаковыми
        if (sign(v0, v1) != sign(v1, v2) || sign(v1, v2) != sign(v2, v0)) {
            return false;
        }
        return true;
    }

    // Вычисление площади треугольника через векторное произведение
    double area() const {
        Point3D v0 = p2-p1;
        Point3D v1 = p3-p1;
        return v0.cross(v1).distanceTo({0, 0, 0}) / 2;
    }

    double sideLength(const Point3D& a, const Point3D& b) const {
        return a.distanceTo(b);
    }

    double isEquilateral() const {
        double side1 = sideLength(p1, p2);
        double side2 = sideLength(p2, p3);
        double side3 = sideLength(p3, p1);

        // Находим максимальную и минимальную длину
        double maxSide = std::max({side1, side2, side3});
        double minSide = std::min({side1, side2, side3});

        // Возвращаем относительное отклонение
        return (maxSide - minSide) / maxSide; // Чем меньше значение, тем более равносторонний треугольник
    }

    Triangle improveEquilateral(double delta = 1e-4, double tolerance = 1e-3, bool inplace = true) {
    Triangle improvedTriangle = *this; // Создаем копию текущего треугольника
    double deviation = improvedTriangle.isEquilateral(); // Текущее отклонение

    // Пока отклонение больше допустимого, продолжаем корректировать вершины
    while (deviation > tolerance) {
        double side1 = improvedTriangle.sideLength(improvedTriangle.p1, improvedTriangle.p2);
        double side2 = improvedTriangle.sideLength(improvedTriangle.p2, improvedTriangle.p3);
        double side3 = improvedTriangle.sideLength(improvedTriangle.p3, improvedTriangle.p1);

        double maxSide = std::max({side1, side2, side3});
        if (inplace) {
            // Если inplace == true, изменяем текущий объект
            *this = improvedTriangle;
            return *this;
        } else {
            // Иначе возвращаем улучшенный треугольник
            return improvedTriangle;
        }
    }

    return improvedTriangle;
}


    void print() const {
        std::cout << "Triangle points:\n";
        p1.print();
        p2.print();
        p3.print();
    }
};

//Класс для создания и хранения треугольников
class Triangles {
public:
    std::vector<Triangle> triangles;
    void addTriangle(const Triangle& triangle) {
        triangles.push_back(triangle);
    }
    //метод для соединения 3 ближайших точек в треугольник
    void connectClosestPoints() {
        for (int i = 0; i < triangles.size(); i++) {
            for (int j = i + 1; j < triangles.size(); j++) {
                for (int k = j + 1; k < triangles.size(); k++) {
                    if (triangles[i].contains(triangles[j].p1) && triangles[i].contains(triangles[k].p1)) {
                        triangles[i] = triangles[i].improveEquilateral();
                        triangles[j] = triangles[j].improveEquilateral();
                        triangles[k] = triangles[k].improveEquilateral();
                    }
                }
            }
        }
    }
};


int main() {
    Point3D a(0, 0, 6);
    Point3D b(1, 0, 0);
    Point3D c(0, 3, 0); // Равносторонний треугольник


    Triangle triangle(a, b, c); // Создаем треугольник

    double deviation = triangle.isEquilateral();
    std::cout << "Before deviation from equilateral: " << deviation << std::endl;

    triangle.improveEquilateral().print();
    deviation = triangle.isEquilateral();
    std::cout << "After deviation from equilateral: " << deviation << std::endl;
    return 0;
}