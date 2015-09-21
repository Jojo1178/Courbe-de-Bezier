#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        //Constructeurs et destructeur
        Point();
        Point(float x, float y, float z);
        Point(const Point &p);
        virtual ~Point();

        //Operateurs
        Point &operator=(const Point &p);
        int operator==(const Point &p) const;
        int operator!=(const Point &p) const;
        int operator<(const Point &p) const;

        //Getters et Setters
        float Getx() const;
        void Setx(const float value);
        float Gety()const;
        void Sety(const float value);
        float Getz() const;
        void Setz(const float value);

    protected:
    private:
        float _x, _y, _z;
};

#endif // POINT_H
