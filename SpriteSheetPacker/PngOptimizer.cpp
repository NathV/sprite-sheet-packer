#include "PngOptimizer.h"
#include "opnglib/include/opnglib/opnglib.h"
#include <QtDebug>
#include <QtCore>

OptiPngOptimizer::OptiPngOptimizer(int optLevel) {
    _optLevel = optLevel;

    memset(&options, 0, sizeof(options));

    options.optim_level = _optLevel;
    options.interlace = -1;

    optimizer.store(opng_create_optimizer());
    transformer.store(opng_create_transformer());

    opng_set_options(optimizer.load(), &options);

    opng_set_transformer(optimizer.load(), transformer.load());
}

OptiPngOptimizer::~OptiPngOptimizer() {
    opng_destroy_optimizer(optimizer.load());
    opng_destroy_transformer(transformer.load());
}

bool OptiPngOptimizer::optimizeFiles(QList<QString> fileNames) {

    for(const QString& fileName : fileNames) {
        if (!optimizeFile(fileName)) {
            continue;
        }
    }

    return true;
}

bool OptiPngOptimizer::optimizeFile(const QString& fileName) {

    if (opng_optimize_file(optimizer.load(),
                           fileName.toStdString().c_str(),
                           fileName.toStdString().c_str(),
                           NULL) == -1) {
        return false;
    }

    return true;
}

bool OptiPngOptimizer::setOptions(int optLevel) {
    _optLevel = optLevel;

    options.optim_level = _optLevel;

    if (opng_set_options(optimizer, &options) < 0) {
        return false;
    }

    return true;
}
