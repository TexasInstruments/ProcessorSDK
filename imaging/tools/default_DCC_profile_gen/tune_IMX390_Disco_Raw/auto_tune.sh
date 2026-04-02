
echo '>>>> IMX390 Disco'

cd ../scripts
rm -rf ../tune_IMX390_Disco_Raw/dcc_xmls
python3 ctt_def_xml_gen.py ../tune_IMX390_Disco_Raw/imx390_cfg.txt
cd -

