/*
 * Copyright (c) 2010 WiYun Inc.
 * Author: luma(stubma@gmail.com)
 *
 * For all entities this program is free software; you can redistribute
 * it and/or modify it under the terms of the 'WiEngine' license with
 * the additional provision that 'WiEngine' must be credited in a manner
 * that can be be observed by end users, for example, in the credits or during
 * start up. (please find WiEngine logo in sdk's logo folder)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "wyLines.h"
#include "wyAttribute.h"
#include "wyLog.h"

wyLines::wyLines() {
	// the mode should be line strip
	m_mode = LINE_STRIP;

	// create buffer
	m_buf = wyBuffer::makeCustom(sizeof(Vertex));

	// connect attribute
	connectAttribute(wyAttribute::NAME[wyAttribute::POSITION], m_buf, 0, 3);
	connectAttribute(wyAttribute::NAME[wyAttribute::COLOR], m_buf, sizeof(kmVec3) + sizeof(kmVec2), 4);
}

wyLines::~wyLines() {
}

wyLines* wyLines::makeBezier(wyBezierConfig& c, int segments) {
	// create lines
	wyLines* lines = WYNEW wyLines();

	// help variable
	Vertex v;
	kmVec4Fill(&v.color, 1, 1, 1, 1);

	// fill every vertex
	float step = 1.f / segments;
	float t = 0;
	for(int i = 0; i <= segments; i++) {
		wyPoint p = wybcPointAt(c, t);
		kmVec3Fill(&v.pos, p.x, p.y, 0);
		lines->m_buf->append(&v, 1);
		t += step;
	}

	// return
	return (wyLines*)lines->autoRelease();
}

wyLines* wyLines::makeLagrange(wyLagrangeConfig& c, int segments) {
	// create lines
	wyLines* lines = WYNEW wyLines();

	// help variable
	Vertex v;
	kmVec4Fill(&v.color, 1, 1, 1, 1);

	// fill every vertex
	float step = 1.f / segments;
	float t = 0;
	for(int i = 0; i <= segments; i++) {
		wyPoint p = wylcPointAt(c, t);
		kmVec3Fill(&v.pos, p.x, p.y, 0);
		lines->m_buf->append(&v, 1);
		t += step;
	}

	// return
	return (wyLines*)lines->autoRelease();
}

void wyLines::updateColor(wyColor4B color) {
	// color
	float r = color.r / 255.0f;
	float g = color.g / 255.0f;
	float b = color.b / 255.0f;
	float a = color.a / 255.0f;

	// update every vertex
	Vertex* v = (Vertex*)m_buf->getData();
	int c = m_buf->getElementCount();
	for(int i = 0; i < c; i++) {
		kmVec4Fill(&v[i].color, r, g, b, a);
	}
}

int wyLines::getElementCount() {
	return m_buf->getElementCount();
}
