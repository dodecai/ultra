#pragma once

#include "Engine/Renderer/VertexArray.h"

namespace Ultra {

class GLVertexArray: public VertexArray {
	std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
	std::shared_ptr<IndexBuffer> mIndexBuffer;
	uint32_t mRendererID;

public:
	GLVertexArray();
	virtual ~GLVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

	virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const override;
	virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const override;
};

}
