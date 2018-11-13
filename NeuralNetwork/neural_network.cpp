#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>

using namespace std;


// ****************************** class Data ******************************
class Data {
public:
	Data(const string filename) { m_data_file.open(filename.c_str()); }
	bool is_eof() { return m_data_file.eof(); }
	void get_next_sample(vector<double> &input_vals, vector<double> &output_vals);
	void get_all_samples(vector<vector<double>> &all_input_vals, vector<vector<double>> &all_output_vals);
private:
	ifstream m_data_file;
};


struct Edge {
	double weight;
	double delta_weight;
};


class Neuron;
typedef vector<Neuron> Layer;


// ****************************** class Neuron ******************************
class Neuron {
public:
	Neuron(unsigned num_outputs, unsigned index_in_layer);
	void set_output_val(double val) { m_output_val = val; }
	double output_val() const { return m_output_val; }
	void feed_forward(const Layer &prev_layer);
	void calc_output_gradient(double target_val);
	void calc_hidden_gradient(const Layer &next_layer);
	void update_input_weights(Layer &prev_layer);
private:
	static double eta;  // [0.0 ... 1.0] learning rate
	static double alpha; // [0.0 ... n] momentum
	static double transfer_function(double x);
	static double transfer_function_derivative(double x);
	static double random_weight() { return rand() / double(RAND_MAX); }
	double sum_downstream(const Layer &next_layer) const;
	double m_output_val;
	vector<Edge> m_output_edges;
	unsigned m_index_in_layer;
	double m_gradient; // ¦Ä, actually m_gradient is -gradient
};


// ******************************* class Net *******************************
class Net {
public:
	Net(const vector<unsigned> &topology);
	void feed_forward(const vector<double> &input_vals);
	void back_propagation(const vector<double> &target_vals);
	void get_results(vector<double> &result_vals) const;
	double rmse() const;
private:
	vector<Layer> m_layers;  // m_layers[layer_num][neuron_num]
	double m_error = -1.0;  // training error for one sample
};


// ****************************** util ******************************
bool double_equals(double a, double b, double epsilon = 0.0001)
{
	return std::abs(a - b) < epsilon;
}

void show_vector(string label, vector<double> &v) {
	cout << label;
	for (int i = 0; i < v.size(); ++i) {
		cout << " " << v[i];
	}
	cout << endl;
}


void make_xor_samples(std::string filename = "samples.txt", unsigned samples_cnt = 2000) {
	std::ofstream out(filename);
	for (int i = 0; i < samples_cnt; i++) {
		int n1 = (int)(2.0 * rand() / double(RAND_MAX));
		int n2 = (int)(2.0 * rand() / double(RAND_MAX));
		int t = n1 ^ n2;
		out << "in:  " << n1 << ".0 " << n2 << ".0 " << std::endl;
		out << "out: " << t << ".0" << std::endl;
	}
}

// ****************************** train ******************************
double Neuron::eta = 0.15; // [0.0 ... 1.0] learning rate
double Neuron::alpha = 0.5; // [0.0 ... n] momentum


void train(string training_filename, const vector<unsigned> &topolopy,
				unsigned evaluate_period = 100, bool show_detail = false)
{
	Data train_data(training_filename);
	Net net(topolopy);
	vector<double> input_vals, target_vals, result_vals;

	int pass = 0;
	double recent_error = 0.0;
	while (true) {
		++pass;
		if (show_detail) {
			cout << "pass" << pass << endl;
		}
		
		train_data.get_next_sample(input_vals, target_vals);
		if (train_data.is_eof()) break;
		if (input_vals.size() != topolopy[0] && target_vals.size() != topolopy.back()) {
			cerr << "Error match topolopy and train data" << endl;
			break;
		}

		net.feed_forward(input_vals);
		net.back_propagation(target_vals);

		recent_error += net.rmse();

		if (pass % evaluate_period == 0) {
			recent_error /= evaluate_period;
			cout << "pass " << pass <<", net recent " << evaluate_period
				<< " samples average rmse:" << recent_error << endl;
			recent_error = 0.0;
		}
		
		if (show_detail) {
			net.get_results(result_vals);
			show_vector(": input :", input_vals);
			show_vector(": target:", target_vals);
			show_vector(": result:", result_vals);
			cout << endl;
		}
	}
	cout << endl << "Done..." << endl;
}


int main() {

	make_xor_samples("training_samples.txt", 10000);

	vector<unsigned> topolopy = { 2, 4, 1 };
	train("training_samples.txt", topolopy, 1000);

	return 0;
}

void Data::get_next_sample(vector<double>& input_vals, vector<double>& output_vals)
{
	input_vals.clear();
	output_vals.clear();

	string line;
	getline(m_data_file, line);

	if (line == "") {
		return;
	}

	stringstream in(line);
	string label;
	in >> label;
	assert(label.compare("in:") == 0);

	double v;
	while (in >> v) {
		input_vals.push_back(v);
	}

	getline(m_data_file, line);
	stringstream out(line);
	out >> label;
	assert(label.compare("out:") == 0);

	while (out >> v) {
		output_vals.push_back(v);
	}
}

void Data::get_all_samples(vector<vector<double>>& all_input_vals, vector<vector<double>>& all_output_vals)
{
	all_input_vals.clear();
	all_output_vals.clear();

	vector<double> input_vals, output_vals;
	while (true) {
		get_next_sample(input_vals, output_vals);
		if (is_eof()) break;
		all_input_vals.push_back(input_vals);
		all_output_vals.push_back(output_vals);
	}
}

Neuron::Neuron(unsigned num_outputs, unsigned index_in_layer)
{
	for (unsigned u = 0; u < num_outputs; ++u) {
		m_output_edges.push_back(Edge());
		m_output_edges.back().weight = random_weight();
	}

	m_index_in_layer = index_in_layer;
}

void Neuron::feed_forward(const Layer & prev_layer)
{
	double sum = 0.0;
	// weight sum previous layer's output
	// map the transfer function to weight sum
	// (include bias node from previous layer)
	for (unsigned n = 0; n < prev_layer.size(); ++n) {
		sum += prev_layer[n].output_val() *
			prev_layer[n].m_output_edges[m_index_in_layer].weight;
	}
	m_output_val = Neuron::transfer_function(sum);
}

void Neuron::calc_output_gradient(double target_val)
{
	double delta = target_val - m_output_val;
	m_gradient = delta * Neuron::transfer_function_derivative(m_output_val);
}

void Neuron::calc_hidden_gradient(const Layer & next_layer)
{
	double sum = sum_downstream(next_layer);
	m_gradient = sum * Neuron::transfer_function_derivative(m_output_val);
}

void Neuron::update_input_weights(Layer & prev_layer)
{
	// updated weights are in previous layer neuron Edge container
	for (unsigned n = 0; n < prev_layer.size(); ++n) {
		Neuron &neuron = prev_layer[n];
		double old_delta_weight = neuron.m_output_edges[m_index_in_layer].delta_weight;
		double new_delta_weight =
			// delta weight
			eta
			* neuron.output_val()
			* m_gradient
			// momentum
			+ alpha
			* old_delta_weight;
		neuron.m_output_edges[m_index_in_layer].delta_weight = new_delta_weight;
		neuron.m_output_edges[m_index_in_layer].weight += new_delta_weight;
	}
}

double Neuron::transfer_function(double x)
{
	// return 1.0 / (1.0 + exp(-x));
	// sigmoid function is not encourage used in feed forward neural network(deep learning -cn, p121)
	return tanh(x);
}

double Neuron::transfer_function_derivative(double x)
{
	//double sigmoid = transfer_function(x);
	//return sigmoid * (1 - sigmoid);
	return 1.0 - x * x;
}

double Neuron::sum_downstream(const Layer & next_layer) const
{
	double sum = 0.0;
	for (unsigned n = 0; n < next_layer.size() - 1; ++n) { // bias is last neuron
		sum += m_output_edges[n].weight * next_layer[n].m_gradient;
	}
	return sum;
}

Net::Net(const vector<unsigned>& topology)
{
	for (unsigned layer_idx = 0, num_layers = topology.size(); layer_idx < num_layers; ++layer_idx) {
		m_layers.push_back(Layer());
		// num_outputs of layers[i] is the num_inputs of layers[i+1]
		// num_outputs of last layer if 0
		unsigned num_outputs = layer_idx == num_layers - 1 ? 0 : topology[layer_idx + 1];
		
		// fill neurons to layer and add bias neuron
		for (unsigned neuron_idx = 0; neuron_idx <= topology[layer_idx]; ++neuron_idx) {
			m_layers.back().push_back(Neuron(num_outputs, neuron_idx));
			cout << "layer:" << layer_idx << " new neuron:" << neuron_idx << endl;
		}

		// define last neuron in layer is bias node
		m_layers.back().back().set_output_val(1.0);
	}
}

void Net::feed_forward(const vector<double>& input_vals)
{
	assert(input_vals.size() == m_layers[0].size() - 1);

	for (unsigned i = 0; i < input_vals.size(); ++i) {
		m_layers[0][i].set_output_val(input_vals[i]);
	}

	for (unsigned layer_idx = 1; layer_idx < m_layers.size(); ++layer_idx) {
		Layer & prev_layer = m_layers[layer_idx - 1];
		for (unsigned n = 0; n < m_layers[layer_idx].size() - 1; ++n) {  // except bias neural
			m_layers[layer_idx][n].feed_forward(prev_layer);
		}
	}
}

void Net::back_propagation(const vector<double>& target_vals)
{
	Layer &output_layer = m_layers.back();
	
	// rmse
	m_error = 0.0;
	for (unsigned n = 0; n < output_layer.size() - 1; ++n) {
		double delta = target_vals[n] - output_layer[n].output_val();
		m_error += delta * delta;
	}
	m_error /= output_layer.size() - 1;
	m_error = sqrt(m_error);

	// output layer gradient
	for (unsigned n = 0; n < output_layer.size() - 1; ++n) {
		output_layer[n].calc_output_gradient(target_vals[n]);
	}

	// hidden layer gradient
	for (unsigned layer_idx = m_layers.size() - 2; layer_idx > 0; --layer_idx) {
		Layer &hidden_layer = m_layers[layer_idx];
		Layer &next_layer = m_layers[layer_idx + 1];

		for (unsigned n = 0; n < hidden_layer.size(); ++n) {
			hidden_layer[n].calc_hidden_gradient(next_layer);
		}
	}

	// update connect weights
	for (unsigned layer_idx = m_layers.size() - 1; layer_idx > 0; --layer_idx) {
		Layer &layer = m_layers[layer_idx];
		Layer &prev_layer = m_layers[layer_idx - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n) {
			layer[n].update_input_weights(prev_layer);
		}
	}

}

void Net::get_results(vector<double>& result_vals) const
{
	result_vals.clear();
	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
		result_vals.push_back(m_layers.back()[n].output_val());
	}
}

double Net::rmse() const
{
	if (m_error < 0) {
		throw runtime_error("Cannot get root mean square error without backpropagation");
	}
	return m_error;
}
