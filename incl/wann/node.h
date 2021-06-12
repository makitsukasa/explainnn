#pragma once

namespace enn {
	enum class NodeType {
		Input,
		Output,
		Hidden,
		Bias,
	};

	enum class ActivationFunction {
		Linear,
		UnsignedStep,
		Sine,
		Gausian,
		HyperbolicTangent,
		UnsignedSigmoid,
		Inverse,
		Absolute,
		Relu,
		Cosine,
		Squared,
	};

	class Node {
	private:
		static int lastId;
		int id;
		NodeType nodeType;
		ActivationFunction actFunc;

	public:
		Node(NodeType nodeType = NodeType::Hidden, ActivationFunction actFunc = ActivationFunction::Linear);
	};
}
