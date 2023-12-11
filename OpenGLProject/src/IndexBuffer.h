#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID; // internal buffer id in opengl
	unsigned int m_Count; // Count of indices

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};