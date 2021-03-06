#include <iostream>
#include <random>

#include "pifo_pipeline_stage.h"
#include "pifo_pipeline.h"

int main() {
  try {
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ele_dis(1, 2);

    // Traffic generation
    PIFOPacket test_packet;

    // Single PIFO pipeline stage consisting of
    // 1 priority and queue
    // (every PIFO pipeline stage has 1 calendar queue)
std::vector<PIFOArguments> targetLeft={{1,QueueType::PRIORITY_QUEUE,0}};
std::vector<PIFOArguments> targetRight={{1,QueueType::PRIORITY_QUEUE,1}};
PIFOPipelineStage pifo1(1,
"class",
{{1, {Operation::DEQ,targetLeft}},{2, {Operation::DEQ,targetRight}},},
[] (const auto & x) {if (x("ptr")==666){static std::map<uint32_t, uint32_t> last_fin_time = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}}; auto ret = last_fin_time.at(static_cast<uint32_t>(x("class"))); last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1; return ret;}
uint32_t y =0; return y;});
PIFOPipelineStage pifo2(2,
"class",
{{2, {Operation::TRANSMIT, {}}},{1, {Operation::TRANSMIT, {}}},},
[] (const auto & x) {if (x("class")==2){static std::map<uint32_t, uint32_t> last_fin_time = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}}; auto ret = last_fin_time.at(static_cast<uint32_t>(x("class"))); last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1; return ret;}
if (x("class")==1){static std::map<uint32_t, uint32_t> last_fin_time = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}}; auto ret = last_fin_time.at(static_cast<uint32_t>(x("class"))); last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1; return ret;}
uint32_t y =0; return y;});
std::vector<PIFOArguments> StargetRight={{0,QueueType::PRIORITY_QUEUE,0}};
PIFOPipelineStage pifo2S(1,
"class",
{{2, {Operation::ENQ,StargetRight}},},
[] (const auto & x) {if (x("class")==2){static uint32_t last_time=0;
static uint32_t tokens =0;
uint32_t send_time = x("time");
uint32_t r = 1
uint32_t B=1
uint32_t plen = 1
tokens = std::min(tokens +r*(x("time") - last_time),B) ;//r=1, B=1
if (plen<=tokens){ 
	return send_time;
}
else{
	send_time = x("time") + (plen-tokens)/r;
	tokens = tokens -plen;
	last_time = x("time");
	return send_time;
}}uint32_t y =0; return y;});
PIFOPipeline TBF_pipeline({pifo1,pifo2,pifo2S,});
PIFOPipeline mesh=TBF_pipeline;
for (uint32_t i = 0; i < 10000; i++) {
      if(ele_dis(gen)==1){test_packet("class") = 1; test_packet("slack")=1;}
      else{test_packet("class") = 2; test_packet("slack")=2;}
      test_packet("time")= (int) i;
      test_packet("ptr") = 666;
      if(test_packet("class")==1){mesh.enq(1,QueueType::PRIORITY_QUEUE,0,test_packet,i);
mesh.enq(0,QueueType::PRIORITY_QUEUE,0,test_packet,i);
}
else{mesh.enq(1,QueueType::PRIORITY_QUEUE,1,test_packet,i);
mesh.enq(2,QueueType::CALENDAR_QUEUE,0, test_packet,i);
}
    }

    std::cout << "Finished enqs\n";

    for (uint32_t i = 10000; i < 20000; i++) {
      auto result = mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, i);
      std::cout << "Deq result is \"" << result << "\"" << std::endl;
    }

    assert_exception(mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, 20000).initialized() == false);
  } catch (const std::exception & e) {
    std::cerr << "Caught exception in main " << std::endl << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}