// ================================================================================================
// 
// If not explicitly stated: Copyright (C) 2016, all rights reserved,
//      Rüdiger Göbl 
//		Email r.goebl@tum.de
//      Chair for Computer Aided Medical Procedures
//      Technische Universität München
//      Boltzmannstr. 3, 85748 Garching b. München, Germany
// 
// ================================================================================================

#ifndef __IQDEMODULATORNODE_H__
#define __IQDEMODULATORNODE_H__

#include <memory>
#include <mutex>
#include <tbb/flow_graph.h>

#include "AbstractNode.h"
#include "RecordObject.h"
#include "USImageProperties.h"

#include "IQDemodulator.h"

namespace supra
{
	class USImage;

	class IQDemodulatorNode : public AbstractNode {
	public:
		IQDemodulatorNode(tbb::flow::graph& graph, const std::string & nodeID, bool queueing);

		virtual size_t getNumInputs() { return 1; }
		virtual size_t getNumOutputs() { return 1; }

		virtual tbb::flow::graph_node * getInput(size_t index) {
			if (index == 0)
			{
				return m_node.get();
			}
			return nullptr;
		};

		virtual tbb::flow::graph_node * getOutput(size_t index) {
			if (index == 0)
			{
				return m_node.get();
			}
			return nullptr;
		};

	protected:
		void configurationEntryChanged(const std::string& configKey);
		void configurationChanged();

	private:
		std::shared_ptr<RecordObject> checkTypeAndDemodulate(std::shared_ptr<RecordObject> mainObj);
		template <typename InputType>
		std::shared_ptr<ContainerBase> demodulateTemplated(std::shared_ptr<USImage> inImage);
		void readFrequencyCompoundingSettings();

		void updateImageProperties(std::shared_ptr<const USImageProperties> imageProperties);

		std::unique_ptr<tbb::flow::graph_node> m_node;
		std::mutex m_mutex;

		std::unique_ptr<IQDemodulator> m_demodulator;
		double m_samplingFrequency;
		double m_referenceFrequency;
		double m_cutoffFrequency;
		size_t m_decimationLowpassFilterLength;
		size_t m_frequencyCompoundingBandpassFilterLength;
		std::vector<double> m_frequencyCompoundingReferenceFrequencies;
		std::vector<double> m_frequencyCompoundingBandwidths;
		std::vector<double> m_frequencyCompoundingWeights;
		DataType m_outputType;

		uint32_t m_decimation;
		vec3s m_resultingSize;

		std::shared_ptr<const USImageProperties> m_lastSeenImageProperties;
		std::shared_ptr<const USImageProperties> m_editedImageProperties;
	};
}

#endif //!__IQDEMODULATORNODE_H__
