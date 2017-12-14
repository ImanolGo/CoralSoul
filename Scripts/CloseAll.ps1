
# get Resolume process
$resolume = Get-Process resolume -ErrorAction SilentlyContinue
if ($resolume) {
  # try gracefully first
  $resolume.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$resolume.HasExited) {
    $resolume | Stop-Process -resolume
  }
}
Remove-Variable resolume

# get Ableton process
$ableton = Get-Process ableton -ErrorAction SilentlyContinue
if ($ableton) {
  # try gracefully first
  $ableton.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$ableton.HasExited) {
    $ableton | Stop-Process -Force
  }
}
Remove-Variable ableton

# get CoralSoulApp process
$of = Get-Process of -ErrorAction SilentlyContinue
if ($of) {
  # try gracefully first
  $of.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$of.HasExited) {
    $of | Stop-Process -Force
  }
}
Remove-Variable of