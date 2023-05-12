#include<array>

namespace geo2d {
    namespace orto {


        struct Point {
            double x=0;
            double y=0;
        };

        Point operator-(const Point& lhs, const Point& rhs) {
            return {lhs.x-rhs.x, lhs.y - rhs.y};
        }
        Point operator+(const Point& lhs, const Point& rhs) {
            return { lhs.x + rhs.x, lhs.y + rhs.y };
        }
        Point operator*(const double& lhs, const Point& rhs) {
            return { lhs*rhs.x, lhs*rhs.y };
        }



        //double Distance(const Point& lhs, const Point& rhs);
        
        using Vector2P = std::array<Point, 2>;
        using Vector4P = std::array<Point, 4>;

        class Quad {

        friend Quad Union(const Quad& lhs, const Quad& rhs);
        friend Quad Intersect(const Quad& lhs, const Quad& rhs);

        public:
            Quad()=default;
            Quad(const Vector4P& input):storage_(input){}
            Quad(Vector4P&& input) :storage_(input) {}
            
            void Scale(double coeff);
            Vector4P getPoints(){return storage_;}
        private:
            Vector4P storage_;
            Point CalculateCenter();
        };

        

        class Rect : public Quad {
        public:
            Rect(const Point& first, const Point& second );
        };

        class Square :public Quad {
        public:
            Square(const Point& first, const Point& second);
            Square(const Point& center, const double& edge_size);
        };
        

        Point Quad::CalculateCenter() {
            return 0.5*(storage_[0]+storage_[2]);
        }


        void Quad::Scale(double coeff) {
            Point center=CalculateCenter();
            for (int i = 0; i < 4; ++i) {
                storage_[i]=coeff*(storage_[i]-center)+center;
            }
        }

        
        

    }
}


int main() {
    
}
