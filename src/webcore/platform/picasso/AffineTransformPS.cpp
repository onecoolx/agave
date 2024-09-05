/*
** AffineTransformPS.cpp: AffineTransform implements by picasso.
**
** Copyright(C) 2009 Zhang Ji Peng (onecoolx@gmail.com)
**
*/

#include "config.h"
#include "AffineTransform.h"

#include "IntRect.h"
#include "FloatRect.h"
#include <wtf/MathExtras.h>

namespace WebCore {

AffineTransform::AffineTransform()
	: m(ps_matrix_create())
{
}

AffineTransform::AffineTransform(ps_matrix* matrix)
	: m(matrix)
{
}

AffineTransform::AffineTransform(const AffineTransform& other)
	: m(ps_matrix_create_copy(other.m))
{

}

AffineTransform::~AffineTransform()
{
	ps_matrix_unref(m);
}

AffineTransform::AffineTransform(float a, float b, float c, float d, float e, float f)
	:m(ps_matrix_create_init(a, b, c, d, e, f))
{
}

void AffineTransform::setMatrix(float a, float b, float c, float d, float e, float f)
{
	ps_matrix_init(m, a, b, c, d, e, f);
}

void AffineTransform::map(int x, int y, int* x2, int* y2) const
{
	ps_point p = {x, y};
	ps_matrix_transform_point(m, &p);
	*x2 = (int)round(p.x);
	*y2 = (int)round(p.y);
}

void AffineTransform::map(float x, float y, float* x2, float* y2) const
{
	ps_point p = {x, y};
	ps_matrix_transform_point(m, &p);
	*x2 = p.x;
	*y2 = p.y;
}

IntPoint AffineTransform::mapPoint(const IntPoint& point) const
{
	float x, y;
	map(point.x(), point.y(), &x, &y);
	return IntPoint((int)round(x), (int)round(y));
}

FloatPoint AffineTransform::mapPoint(const FloatPoint& point) const
{
	float x, y;
	map(point.x(), point.y(), &x, &y);
	return FloatPoint(x, y);
}

FloatRect AffineTransform::mapRect(const FloatRect& rect) const
{
	ps_rect r = {rect.x(), rect.y(), rect.width(), rect.height()};
	ps_matrix_transform_rect(m, &r);
	return FloatRect(r.x, r.y, r.w, r.h);
}

IntRect AffineTransform::mapRect(const IntRect& rect) const
{
	ps_rect r = {rect.x(), rect.y(), rect.width(), rect.height()};
	ps_matrix_transform_rect(m, &r);
	return IntRect(r.x, r.y, r.w, r.h);
}

bool AffineTransform::isIdentity() const
{
	return ps_matrix_is_identity(m) ? true : false;
}

float AffineTransform::a() const
{
	float sx, sy;
	ps_matrix_get_scale_factor(m, &sx, &sy);
	return sx;
}

void AffineTransform::setA(float a)
{
	float sx, sy;
	ps_matrix_get_scale_factor(m, &sx, &sy);
	ps_matrix_set_scale_factor(m, a, sy);
}

float AffineTransform::b() const
{
	float shx, shy;
	ps_matrix_get_shear_factor(m, &shx, &shy);
	return shy;
}

void AffineTransform::setB(float b)
{
	float shx, shy;
	ps_matrix_get_shear_factor(m, &shx, &shy);
	ps_matrix_set_shear_factor(m, shx, b);
}

float AffineTransform::c() const
{
	float shx, shy;
	ps_matrix_get_shear_factor(m, &shx, &shy);
	return shx;
}

void AffineTransform::setC(float c)
{
	float shx, shy;
	ps_matrix_get_shear_factor(m, &shx, &shy);
	ps_matrix_set_shear_factor(m, c, shy);
}

float AffineTransform::d() const
{
	float sx, sy;
	ps_matrix_get_scale_factor(m, &sx, &sy);
	return sy;
}

void AffineTransform::setD(float d)
{
	float sx, sy;
	ps_matrix_get_scale_factor(m, &sx, &sy);
	ps_matrix_set_scale_factor(m, sx, d);
}

float AffineTransform::e() const
{
	float tx, ty;
	ps_matrix_get_translate_factor(m, &tx, &ty);
	return tx;
}

void AffineTransform::setE(float e)
{
	float tx, ty;
	ps_matrix_get_translate_factor(m, &tx, &ty);
	ps_matrix_set_translate_factor(m, e, ty);
}

float AffineTransform::f() const
{
	float tx, ty;
	ps_matrix_get_translate_factor(m, &tx, &ty);
	return ty;
}

void AffineTransform::setF(float f)
{
	float tx, ty;
	ps_matrix_get_translate_factor(m, &tx, &ty);
	ps_matrix_set_translate_factor(m, tx, f);
}

void AffineTransform::reset()
{
	ps_matrix_identity(m);
}

AffineTransform& AffineTransform::scale(float sx, float sy)
{
	ps_matrix_scale(m, sx, sy);
	return *this;
}

AffineTransform& AffineTransform::rotate(float d)
{
	ps_matrix_rotate(m, d);
	return *this;
}

AffineTransform& AffineTransform::translate(float tx, float ty)
{
	ps_matrix_translate(m, tx, ty);
	return *this;
}

AffineTransform& AffineTransform::shear(float sx, float sy)
{
	ps_matrix_shear(m, sx, sy);
    return *this;
}

float AffineTransform::det() const
{
	return ps_matrix_get_determinant(m);
}

AffineTransform AffineTransform::inverse() const
{
	ps_matrix* p = ps_matrix_create_copy(m);
	ps_matrix_invert(p);
	return AffineTransform(p);
}

AffineTransform& AffineTransform::operator=(const AffineTransform& other)
{
	if (this == &other)
		return *this;

	if (*this == other)
		return *this;

	ps_matrix_unref(m);
	m = ps_matrix_create_copy(other.m);
	return *this;
}

bool AffineTransform::operator==(const AffineTransform& other) const
{
	return ps_matrix_is_equal(m, other.m) ? true : false;
}

AffineTransform& AffineTransform::operator*=(const AffineTransform& other)
{
	ps_matrix_multiply(m, m, other.m);
	return *this;
}

AffineTransform AffineTransform::operator*(const AffineTransform& other)
{
	ps_matrix* p = ps_matrix_create();
	ps_matrix_multiply(p, m, other.m);
	return AffineTransform(p);
}

}
