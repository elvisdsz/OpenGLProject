#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // internal buffer id in opengl

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};