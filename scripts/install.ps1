#!/usr/bin/env powershell

Set-Alias aws 'C:\Program Files\Amazon\AWSCLIV2\aws.exe'

# Flash application
./mos flash

# Change device ID
$ID=(./mos call sys.getinfo | ConvertFrom-Json | Select mac).mac
./mos config-set device.id=$ID
./mos call sys.reboot

# Populate web server
Get-ChildItem ./web | ForEach-Object {
    $content = Get-Content $_.FullName
    If ($_.Name -eq 'index.html') {
        $content = ($content -replace '{{ DEVICE_ID }}',$ID)
    }
    $bytes = [System.Text.Encoding]::UTF8.GetBytes($content)
    $encoded = [Convert]::ToBase64String($bytes)
    ./mos call fs.put ('{\"filename\":\"/web/' + $_.Name + '\",\"data\":\"' + $encoded + '\"}')
}

# Check existing certificate
$crt = 'devices/aws-' + $ID + '.crt.pem'
If ([System.IO.File]::Exists($crt)) {
    $crtid = ((Get-Content $crt) -match '^ID: (.*)$') -replace 'ID: ',''
    $crtarn = (aws iot describe-certificate --certificate-id $crtid `
        --query 'certificateDescription.certificateArn') -replace '"',''
    aws iot detach-thing-principal --thing-name $ID --principal $crtarn
    aws iot detach-policy --policy-name default --target $crtarn
    aws iot update-certificate --certificate-id $crtid --new-status INACTIVE
    aws iot delete-certificate --certificate-id $crtid
}

# Provision with AWS IoT
./mos aws-iot-setup --aws-region us-east-1 --aws-iot-policy default

## Move certificate files to devices directory
Move-Item ('aws-' + $ID + '.*') 'devices/' -force

# Open console
./mos call sys.reboot
./mos console
