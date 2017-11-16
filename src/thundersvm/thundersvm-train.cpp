//
// Created by jiashuai on 17-9-14.
//


#include <thundersvm/util/log.h>
#include <thundersvm/model/svc.h>
#include <thundersvm/model/svr.h>
#include <thundersvm/model/oneclass_svc.h>
#include <thundersvm/model/nusvc.h>
#include <thundersvm/model/nusvr.h>
#include <thundersvm/util/metric.h>
#include "thundersvm/cmdparser.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    el::Loggers::addFlag(el::LoggingFlag::FixedTimeFormat);
    CMDParser parser;
    parser.parse_command_line(argc, argv);

    DataSet train_dataset;
    train_dataset.load_from_file(parser.svmtrain_input_file_name);
    SvmModel *model = nullptr;
    switch (parser.param_cmd.svm_type) {
        case SvmParam::C_SVC:
            model = new SVC();
            break;
        case SvmParam::NU_SVC:
            model = new NuSVC();
            break;
        case SvmParam::ONE_CLASS:
            model = new OneClassSVC();
            break;
        case SvmParam::EPSILON_SVR:
            model = new SVR();
            break;
        case SvmParam::NU_SVR:
            model = new NuSVR();
            break;
    }

	//todo add this to check_parameter method
    if (parser.param_cmd.svm_type == SvmParam::NU_SVC) {
        train_dataset.group_classes();
        for (int i = 0; i < train_dataset.n_classes(); ++i) {
            int n1 = train_dataset.count()[i];
            for (int j = i + 1; j < train_dataset.n_classes(); ++j) {
                int n2 = train_dataset.count()[j];
                if (parser.param_cmd.nu * (n1 + n2) / 2 > min(n1, n2)) {
                    printf("specified nu is infeasible\n");
                    return 1;
                }
            }
        }
    }

#ifdef USE_CUDA
    CUDA_CHECK(cudaSetDevice(parser.gpu_id));
#endif

    vector<float_type> predict_y, test_y;
    if (parser.do_cross_validation) {
        vector<float_type> test_predict = model->cross_validation(train_dataset, parser.param_cmd, parser.nr_fold);
	   uint dataset_size = test_predict.size() / 2;
	   test_y.insert(test_y.end(), test_predict.begin(), test_predict.begin() + dataset_size);
	   predict_y.insert(predict_y.end(), test_predict.begin() + dataset_size, test_predict.end());
    } else {
        model->train(train_dataset, parser.param_cmd);
        model->save_to_file(parser.model_file_name);
//    	predict_y = model->predict(train_dataset.instances(), 10000);
		test_y = train_dataset.y();
    }

    //perform svm testing
//    Metric *metric = nullptr;
//    switch (parser.param_cmd.svm_type) {
//        case SvmParam::C_SVC:
//        case SvmParam::NU_SVC: {
//            metric = new Accuracy();
//            break;
//        }
//        case SvmParam::EPSILON_SVR:
//        case SvmParam::NU_SVR: {
//            metric = new MSE();
//            break;
//        }
//        case SvmParam::ONE_CLASS: {
//        }
//    }
//    if (metric) {
//        LOG(INFO) << metric->name() << " = " << metric->score(predict_y, test_y);
//    }
//    return 0;
}

