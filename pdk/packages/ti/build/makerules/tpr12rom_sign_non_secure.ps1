#
# windows script to add x509 certificate to binary/ELF
param
(
	[string]$BIN,
	[string]$KEY,
	[string]$OUTPUT = $null,
	[ValidateSet('R5','HSM')][string]$CERT_SIGN = 'R5'
)

#paths
$SCRIPT_DIR = split-path -parent $MyInvocation.MyCommand.Definition
$LOADADDR = '0x10200000'



$WORK_DIR = [io.path]::GetFileNameWithoutExtension($BIN)

#create working dir from input file
#to allow parallel makes
$WORK_DIR="$SCRIPT_DIR\$WORK_DIR"
New-Item -ItemType Directory -Force -Path $WORK_DIR | Out-Null

#variables
$SHA = 'sha512'
$TEMP_X509="$WORK_DIR\x509-temp.cert"
$CERT="$WORK_DIR\$CERT_SIGN" + '-cert.bin'
$VALID_CERT_SIGNS=@('R5', 'HSM')
$X509_TEMPPLATE="$SCRIPT_DIR\tpr12_x509template.txt"


# setup default output path if not specified
if ( $OUTPUT.length -eq  0 ) {
	$OUTPUT="$WORK_DIR\x509-firmware.bin"
}

#check if openssl is present
Write-Host "Checking for OpenSSL..."
try { Invoke-Expression "openssl version" }
catch { Write-Host "Not found! Please install OpenSSL" 
    exit 1
}

$sha_oids = @{}
$sha_oids.sha256='2.16.840.1.101.3.4.2.1'
$sha_oids.sha384='2.16.840.1.101.3.4.2.2'
$sha_oids.sha512='2.16.840.1.101.3.4.2.3'
$sha_oids.sha224='2.16.840.1.101.3.4.2.4'

$options_help = @{}
function usage() {

	if ( $args.count -ne 0 ) {
		Write-Host "ERROR: $args"
	}

	Write-Host -NoNewline "Usage: $(split-path $MyInvocation.PSCommandPath -Leaf) "
	foreach($option in $options_help.Keys) {
		$param_short_desc = $options_help.$option.split(':')[0]
		Write-Host -NoNewline "[-$option $param_short_desc] "
	}
	Write-Host ""
	Write-Host "Where:"
	foreach($option in $options_help.Keys) {
		$param_short_desc, $param_long_desc = $options_help.$option.split(':')
		Write-Host "   -$option $param_short_desc$param_long_desc"
	}
	Write-Host 'Examples of usage:-'
	Write-Host '# Generate x509 certificate from bin'
	Write-Host "	$(split-path $MyInvocation.PSCommandPath -Leaf) -b sbl_qspi_img_mcu1_0_release.bin -c R5 -k tpr12_gpkey.pem "
}

$options_help.b="bin_file:`t`ttBin file that needs to be signed"
$options_help.k="key_file:`t`tFile with key inside it. If not provided script generates a random key."


if (  $BIN.length -eq  0  ) {
	usage "Input bin file missing"
	exit 1
}

#Generate random key if user doesn't provide a key.
if ( $KEY.length -eq 0 ) {
	usage "key file missing"
	exit 1
}

if ( "$CERT_SIGN" -eq 'R5' ) {
	$BOOTCORE_ID=16
	$CERT_TYPE=1
	$BOOTCORE_OPTS=0
} else {
	$BOOTCORE_ID=0
	$BOOTCORE_OPTS=0
	$CERT_TYPE=2
}

$SHA_OID=$sha_oids.$SHA
$SHA_VAL = $(Invoke-Expression "openssl dgst -$SHA -hex $BIN") -replace '.*= ', ''
$BIN_SIZE=(Get-Item $BIN).length
$ADDR = ([Int32]::Parse($LOADADDR.split('x')[1], 'HexNumber')).ToString('X8')

function gen_cert() {
		Write-Host "$CERT_SIGN Certificate being generated :"
		Write-Host "`tX509_CFG = $TEMP_X509"
		Write-Host "`tKEY = $KEY"
		Write-Host "`tBIN = $BIN"
		Write-Host "`tCERT TYPE = $CERT_SIGN"
		Write-Host "`tCORE ID = $CERT_TYPE"
		Write-Host "`tLOADADDR = 0x$ADDR"
		Write-Host "`tIMAGE_SIZE = $BIN_SIZE"
	$SSL_CONF_FILE = (Get-Content -Raw $X509_TEMPPLATE) | ForEach-Object {
				$_.replace("TEST_IMAGE_LENGTH", "$BIN_SIZE").
				replace("TEST_IMAGE_SHA_OID", "$SHA_OID").
				replace("TEST_IMAGE_SHA512", "$SHA_VAL").
				replace("TEST_CERT_TYPE", "$CERT_TYPE").
				replace("TEST_BOOT_CORE_ID", "$BOOTCORE_ID").
				replace("TEST_BOOT_CORE_OPTS", "$BOOTCORE_OPTS").
				replace("TEST_BOOT_ADDR", "$ADDR")
				}
	[IO.File]::WriteAllText($TEMP_X509, $SSL_CONF_FILE)
	Invoke-Expression "openssl req -new -x509 -key $KEY -nodes -outform DER -out $CERT -config $TEMP_X509 -$SHA -days 365"
}

gen_cert
Get-Content $CERT, $BIN -Enc Byte -Read 512 | Set-Content $OUTPUT  -Enc Byte

Write-Host "SUCCESS: Image $OUTPUT generated. Good to boot"

#Remove all intermediate files
Remove-Item $TEMP_X509
Remove-Item $CERT
Remove-Item $WORK_DIR -Recurse