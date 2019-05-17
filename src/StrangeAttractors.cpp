#include "Nohmad.hpp"

struct LorenzAttractor {
	float sigma, beta, rho, pitch; // Params
	float x, y, z; // Out

	static constexpr float DEFAULT_SIGNMA_VALUE = 10.0f;
	static constexpr float DEFAULT_BETA_VALUE = 8.0f / 3.0f;
	static constexpr float DEFAULT_RHO_VALUE = 28.0f;
	static constexpr float DEFAULT_PITCH_VALUE = 0.5f;

	LorenzAttractor() :
		sigma(DEFAULT_SIGNMA_VALUE), beta(DEFAULT_BETA_VALUE), rho(DEFAULT_RHO_VALUE), pitch(DEFAULT_PITCH_VALUE),
		x(1.0f), y(1.0f), z(1.0f) {}

	void process(float dt) {
		float dx = sigma * (y - x);
		float dy = x * (rho - z) - y;
		float dz = (x * y) - (beta * z);

		x += dx * dt * pitch * 375.0f;
		y += dy * dt * pitch * 375.0f;
		z += dz * dt * pitch * 375.0f;
	}
};

struct RosslerAttractor {
	float a, b, c, pitch; // Params
	float x, y, z; // Out

	static constexpr float DEFAULT_A_VALUE = 0.2f;
	static constexpr float DEFAULT_B_VALUE = 0.2f;
	static constexpr float DEFAULT_C_VALUE = 5.7f;
	static constexpr float DEFAULT_PITCH_VALUE = 0.5f;

	RosslerAttractor() :
		a(DEFAULT_A_VALUE), b(DEFAULT_B_VALUE), c(DEFAULT_C_VALUE), pitch(DEFAULT_PITCH_VALUE),
		x(1.0f), y(1.0f), z(1.0f) {}

	void process(float dt) {
		float dx = -y - z;
		float dy = x + (a * y);
		float dz = b + z * (x - c);

		x += dx * dt * pitch * 2910.0f;
		y += dy * dt * pitch * 2910.0f;
		z += dz * dt * pitch * 2910.0f;
	}
};

struct StrangeAttractors : Module {
	enum ParamIds {
		LORENZ_SIGMA_PARAM,
		LORENZ_BETA_PARAM,
		LORENZ_RHO_PARAM,
		LORENZ_PITCH_PARAM,
		ROSSLER_A_PARAM,
		ROSSLER_B_PARAM,
		ROSSLER_C_PARAM,
		ROSSLER_PITCH_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
		LORENZ_SIGMA_INPUT,
		LORENZ_BETA_INPUT,
		LORENZ_RHO_INPUT,
		LORENZ_PITCH_INPUT,
		ROSSLER_A_INPUT,
		ROSSLER_B_INPUT,
		ROSSLER_C_INPUT,
		ROSSLER_PITCH_INPUT,
		NUM_INPUTS
	};

	enum OutputIds {
		LORENZ_X_OUTPUT,
		LORENZ_Y_OUTPUT,
		ROSSLER_X_OUTPUT,
		ROSSLER_Y_OUTPUT,
		NUM_OUTPUTS
	};

	LorenzAttractor lorenz;
	RosslerAttractor rossler;

	static constexpr float LORENZ_SIGMA_PARAM_MIN = 3.0f;
	static constexpr float LORENZ_SIGMA_PARAM_MAX = 30.0f;
	static constexpr float LORENZ_BETA_PARAM_MIN = 0.5f;
	static constexpr float LORENZ_BETA_PARAM_MAX = 3.0f;
	static constexpr float LORENZ_RHO_PARAM_MIN = 13.0f;
	static constexpr float LORENZ_RHO_PARAM_MAX = 80.0f;
	static constexpr float LORENZ_PITCH_PARAM_MIN = 0.001f;
	static constexpr float LORENZ_PITCH_PARAM_MAX = 1.0f;

	static constexpr float ROSSLER_A_PARAM_MIN = 0.0f;
	static constexpr float ROSSLER_A_PARAM_MAX = 0.2f;
	static constexpr float ROSSLER_B_PARAM_MIN = 0.1f;
	static constexpr float ROSSLER_B_PARAM_MAX = 1.0f;
	static constexpr float ROSSLER_C_PARAM_MIN = 3.0f;
	static constexpr float ROSSLER_C_PARAM_MAX = 12.0f;
	static constexpr float ROSSLER_PITCH_PARAM_MIN = 0.001f;
	static constexpr float ROSSLER_PITCH_PARAM_MAX = 1.0f;

	StrangeAttractors() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
		configParam(LORENZ_SIGMA_PARAM, LORENZ_SIGMA_PARAM_MIN, LORENZ_SIGMA_PARAM_MAX, LorenzAttractor::DEFAULT_SIGNMA_VALUE);
		configParam(LORENZ_BETA_PARAM, LORENZ_BETA_PARAM_MIN, LORENZ_BETA_PARAM_MAX, LorenzAttractor::DEFAULT_BETA_VALUE);
		configParam(LORENZ_RHO_PARAM, LORENZ_RHO_PARAM_MIN, LORENZ_RHO_PARAM_MAX, LorenzAttractor::DEFAULT_RHO_VALUE);
		configParam(LORENZ_PITCH_PARAM, LORENZ_PITCH_PARAM_MIN, LORENZ_PITCH_PARAM_MAX, LorenzAttractor::DEFAULT_PITCH_VALUE);
		configParam(ROSSLER_A_PARAM, ROSSLER_A_PARAM_MIN, ROSSLER_A_PARAM_MAX, RosslerAttractor::DEFAULT_A_VALUE);
		configParam(ROSSLER_B_PARAM, ROSSLER_B_PARAM_MIN, ROSSLER_B_PARAM_MAX, RosslerAttractor::DEFAULT_B_VALUE);
		configParam(ROSSLER_C_PARAM, ROSSLER_C_PARAM_MIN, ROSSLER_C_PARAM_MAX, RosslerAttractor::DEFAULT_C_VALUE);
		configParam(ROSSLER_PITCH_PARAM, ROSSLER_PITCH_PARAM_MIN, ROSSLER_PITCH_PARAM_MAX, RosslerAttractor::DEFAULT_PITCH_VALUE);
	}

	void process(const ProcessArgs &args) override;
};

void StrangeAttractors::process(const ProcessArgs &args) {
	if (outputs[LORENZ_X_OUTPUT].isConnected() || outputs[LORENZ_Y_OUTPUT].isConnected()) {
		lorenz.sigma = clamp(params[LORENZ_SIGMA_PARAM].getValue() + inputs[LORENZ_SIGMA_INPUT].getVoltage() * 0.1f, LORENZ_SIGMA_PARAM_MIN, LORENZ_SIGMA_PARAM_MAX);
		lorenz.beta = clamp(params[LORENZ_BETA_PARAM].getValue() + inputs[LORENZ_BETA_INPUT].getVoltage() * 0.1f, LORENZ_BETA_PARAM_MIN, LORENZ_BETA_PARAM_MAX);
		lorenz.rho = clamp(params[LORENZ_RHO_PARAM].getValue() + inputs[LORENZ_RHO_INPUT].getVoltage() * 0.1f, LORENZ_RHO_PARAM_MIN, LORENZ_RHO_PARAM_MAX);
		lorenz.pitch = clamp(params[LORENZ_PITCH_PARAM].getValue() + inputs[LORENZ_PITCH_INPUT].getVoltage() * 0.1f, LORENZ_PITCH_PARAM_MIN, LORENZ_PITCH_PARAM_MAX);

		lorenz.process(1.0f / args.sampleRate);
		outputs[LORENZ_X_OUTPUT].setVoltage(5.0f * 0.044f * lorenz.x);
		outputs[LORENZ_Y_OUTPUT].setVoltage(5.0f * 0.0328f * lorenz.y);
	}

	if (outputs[ROSSLER_X_OUTPUT].isConnected() || outputs[ROSSLER_Y_OUTPUT].isConnected()) {
		rossler.a = clamp(params[ROSSLER_A_PARAM].getValue() + inputs[ROSSLER_A_INPUT].getVoltage() * 0.1, ROSSLER_A_PARAM_MIN, ROSSLER_A_PARAM_MAX);
		rossler.b = clamp(params[ROSSLER_B_PARAM].getValue() + inputs[ROSSLER_B_INPUT].getVoltage() * 0.1, ROSSLER_B_PARAM_MIN, ROSSLER_B_PARAM_MAX);
		rossler.c = clamp(params[ROSSLER_C_PARAM].getValue() + inputs[ROSSLER_C_INPUT].getVoltage() * 0.1, ROSSLER_C_PARAM_MIN, ROSSLER_C_PARAM_MAX);
		rossler.pitch = clamp(params[ROSSLER_PITCH_PARAM].getValue() + inputs[ROSSLER_PITCH_INPUT].getVoltage() * 0.1, ROSSLER_PITCH_PARAM_MIN, ROSSLER_PITCH_PARAM_MAX);

		rossler.process(1.0f / args.sampleRate);
		outputs[ROSSLER_X_OUTPUT].setVoltage(5.0f * 0.054f * rossler.x);
		outputs[ROSSLER_Y_OUTPUT].setVoltage(5.0f * 0.0569f * rossler.y);
	}
}


struct StrangeAttractorsWidget : ModuleWidget {
	StrangeAttractorsWidget(StrangeAttractors *module);
};

StrangeAttractorsWidget::StrangeAttractorsWidget(StrangeAttractors *module) {
		setModule(module);
	box.size = Vec(15 * 12, 380);

	setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/StrangeAttractors.svg")));

	addParam(createParam<Davies1900hBlackKnob>(Vec(8, 45), module, StrangeAttractors::LORENZ_SIGMA_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(50, 45), module, StrangeAttractors::LORENZ_BETA_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 45), module, StrangeAttractors::LORENZ_RHO_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(135, 45), module, StrangeAttractors::LORENZ_PITCH_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(8, 237), module, StrangeAttractors::ROSSLER_A_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(50, 237), module, StrangeAttractors::ROSSLER_B_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(92.5, 237), module, StrangeAttractors::ROSSLER_C_PARAM));
	addParam(createParam<Davies1900hBlackKnob>(Vec(135, 237), module, StrangeAttractors::ROSSLER_PITCH_PARAM));

	addInput(createInput<PJ301MPort>(Vec(12.5, 110), module, StrangeAttractors::LORENZ_SIGMA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 110), module, StrangeAttractors::LORENZ_BETA_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 110), module, StrangeAttractors::LORENZ_RHO_INPUT));
	addInput(createInput<PJ301MPort>(Vec(140, 110), module, StrangeAttractors::LORENZ_PITCH_INPUT));
	addInput(createInput<PJ301MPort>(Vec(12.5, 300), module, StrangeAttractors::ROSSLER_A_INPUT));
	addInput(createInput<PJ301MPort>(Vec(55, 300), module, StrangeAttractors::ROSSLER_B_INPUT));
	addInput(createInput<PJ301MPort>(Vec(97.5, 300), module, StrangeAttractors::ROSSLER_C_INPUT));
	addInput(createInput<PJ301MPort>(Vec(140, 300), module, StrangeAttractors::ROSSLER_PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(12.5, 154), module, StrangeAttractors::LORENZ_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(140, 154), module, StrangeAttractors::LORENZ_Y_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(12.5, 345), module, StrangeAttractors::ROSSLER_X_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(140, 345), module, StrangeAttractors::ROSSLER_Y_OUTPUT));
}

Model *modelStrangeAttractors = createModel<StrangeAttractors, StrangeAttractorsWidget>("StrangeAttractors");
