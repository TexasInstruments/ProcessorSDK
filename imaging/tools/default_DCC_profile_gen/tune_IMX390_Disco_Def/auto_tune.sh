
echo '>>>> IMX390 Disco Def'

cd ../scripts
rm -rf ../tune_IMX390_Disco_Def/dcc_xmls
python3 ctt_def_xml_gen.py ../tune_IMX390_Disco_Def/imx390_cfg.txt
cd -

