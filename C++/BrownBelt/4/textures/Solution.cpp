#include "Common.h"

using namespace std;

class Shape : public IShape {
public:
    void SetPosition(Point position) final {
        position_ = position;
    }
    Point GetPosition() const final {
        return position_;
    }

    void SetSize(Size size) final {
        size_ = size;
    }
    Size GetSize() const final {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) final {
        texture_ = texture;
        if(texture_) {
            const auto& image = texture_->GetImage();
            if(image.size() > 0 && image[0].size() > 0) {
                texture_size.height = static_cast<int>(image.size());
                texture_size.width = static_cast<int>(image[0].size());
            }
        }
    }
    ITexture *GetTexture() const final {
        return texture_.get();
    }

protected:
    shared_ptr<ITexture> texture_;
    Size texture_size{0, 0};
    Point position_ = {0, 0};
    Size size_ = {1, 1};
};

class Rectangle : public Shape
{
public:
    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Rectangle>(*this);
    }

    void Draw(Image &image) const override {
        for(int row = 0; row < size_.height; ++row) {
            int y = position_.y + row;
            if(y >= image.size())
                break;
            for(int col = 0; col < size_.width; ++col) {
                int x = position_.x + col;
                if(x >= image[y].size())
                    break;
                if(row < texture_size.height && col < texture_size.width) {
                    image[y][x] = texture_->GetImage()[row][col];
                } else {
                    image[y][x] = '.';
                }
            }
        }
    }
};

class Ellipse : public Shape {
public:
    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Ellipse>(*this);
    }

    void Draw(Image &image) const override {
        for(int row = 0; row < size_.height; ++row) {
            int y = position_.y + row;
            for(int col = 0; col < size_.width; ++col) {
                if(IsPointInEllipse({col, row}, size_)) {
                    int x = position_.x + col;
                    if(row < texture_size.height && col < texture_size.width) {
                        image[y][x] = texture_->GetImage()[row][col];
                    } else {
                        image[y][x] = '.';
                    }
                }
            }
        }
    }
};

unique_ptr<IShape> MakeShape(ShapeType shape_type)
{
    switch (shape_type) {
    case ShapeType::Rectangle:
        return make_unique<Rectangle>();

    case ShapeType::Ellipse:
        return make_unique<Ellipse>();
    }
    return {};
}
