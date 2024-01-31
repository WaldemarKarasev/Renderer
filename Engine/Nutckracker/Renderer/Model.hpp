#pragma once

#include "Core.h"
#include <memory>
#include "Buffer.hpp"

namespace NK {

	class NK_API Model
	{
	public:
		virtual ~Model() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Draw() const = 0;

		virtual void BuildModel(const Builder& builder)  = 0;

		static Model* Create(const Builder& builder);
	};

}