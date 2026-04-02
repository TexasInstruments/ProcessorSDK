import vlab

# component
Cppi_generator = vlab.component("ppi_generator",
                        module="keystone.csi2_tb.ppi_generator",
                        visibility="opaque")
# instance of ppi_generator
ppi_generator0 = vlab.instantiate(Cppi_generator, "PPI_GENERATOR0")

vlab.connect((ppi_generator0, "data_out"), ("platform", "csi_rx0_ppi_in"))

vlab.stub(ppi_generator0)
