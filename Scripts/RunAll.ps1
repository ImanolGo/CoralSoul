echo "Run All Script "
echo "Starting ... "


while($true)
{
    # get Resolume process
    $resolume = Get-Process Arena -ErrorAction SilentlyContinue
    if (-Not ($resolume)) {
        echo "Process Arena Not Found"
        echo "Starting Resolume Arena"
        Invoke-Item C:\Users\user\CoralSoul\Resolume\*.avc
        Start-Sleep -Seconds 5
    }
    Remove-Variable resolume

    # get Ableton process
    $ableton = Get-Process "Ableton Live 9 Suite" -ErrorAction SilentlyContinue
    if (-Not ($ableton)) {
      echo "Process Ableton Not Found"
	  echo "Deleting Ableton Config files"
	  Remove-Item –path "c:\Users\user\AppData\Roaming\Ableton\Live 9.6.1\Preferences\Preferences.cfg" –recurse
	  Remove-Item –path "c:\Users\user\AppData\Roaming\Ableton\Live 9.6.1\Preferences\Template.als" –recurse
	  Remove-Item –path "c:\Users\user\AppData\Roaming\Ableton\Live 9.6.1\Preferences\Undo.cfg" –recurse
      echo "Starting Ableton"
      Invoke-Item C:\Users\user\CoralSoul\Ableton\CoralSoulAbleton\CoralSoulAbleton.als
      Start-Sleep -Seconds 5
    }
    Remove-Variable ableton

     # get CoralSoulApp process
     $unity = Get-Process CoralSoulUnity -ErrorAction SilentlyContinue
     if (-Not ($unity)) {
      echo "Process Unity Not Found"
      echo "Starting Unity"
       #Invoke-Item C:\Users\user\CoralSoul\Unity\CoralSoulUnity.exe
       Start-Sleep -Seconds 5
     }
     Remove-Variable unity

    # get CoralSoulApp process
    $of = Get-Process CoralSoulApp -ErrorAction SilentlyContinue
    if (-Not ($of)) {
      echo "Process OF Not Found"
      echo "Starting OF"
      Invoke-Item C:\Users\user\CoralSoul\OF\CoralSoulApp.exe
      Start-Sleep -Seconds 5
    }
    else{

    }
    Remove-Variable of


    Start-Sleep -Seconds 15
}






