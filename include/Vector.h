#ifndef VECTOR_H
#define VECTOR_H


class Vector
{
    public:
        //Constructeurs et Destructeur
        Vector();
        Vector(float x, float y, float z);
        Vector(const Vector & p);
        virtual ~Vector();

        //Operateurs
        Vector &operator=(const Vector &v);
        int operator==(const Vector &v) const;

        double GetNorm();

        //Getters et Setters
        float Getx() const;
        void Setx(const float value);
        float Gety() const;
        void Sety(const float value);
        float Getz() const;
        void Setz(const float value);
    protected:
    private:
        float _x,_y,_z;
};

#endif // VECTOR_H
