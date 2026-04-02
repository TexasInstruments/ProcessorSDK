
let common = system.getScript("/common");

const udma_config = [
    {
        name: "BCDMA_0",
        type: "BCDMA",
        numBlkCopyCh: 28,
    },
    {
        name: "BCDMA_1",
        type: "BCDMA",
        numBlkCopyCh: 0,
    },
    {
        name: "PKTDMA_0",
        type: "PKTDMA",
        numBlkCopyCh: 0,
    },
];

const udma_config_c7 = [
    {
        name: "BCDMA_0",
        type: "BCDMA",
        numBlkCopyCh: 28,
    },
    {
        name: "BCDMA_1",
        type: "BCDMA",
        numBlkCopyCh: 0,
    },
    {
        name: "PKTDMA_0",
        type: "PKTDMA",
        numBlkCopyCh: 0,
    },
    {
        name: "C7X_DRU_0",
        type: "C7X_DRU",
        numBlkCopyCh: 32,
    },
    {
        name: "C7X_DRU_1",
        type: "C7X_DRU",
        numBlkCopyCh: 32,
    },
];

function getConfigArr() {
    let cpu = common.getSelfSysCfgCoreName();
    let config = udma_config;
    if (cpu.match(/c75*/))
    {
        config = udma_config_c7;
    }
    else
    {
        config = udma_config;
    }
    return config;
}

exports = {
    getConfigArr,
};