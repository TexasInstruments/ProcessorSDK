
echo '>>>> imx390 RCM'

cd ../scripts
rm -rf ../tune_IMX390_RCM/dcc_xmls
python3 ctt_def_xml_gen.py ../tune_IMX390_RCM/imx390_cfg.txt
cd -

