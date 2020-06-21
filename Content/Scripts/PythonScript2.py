import unreal_engine as ue
from unreal_engine.enums import EPixelFormat
import matplotlib.pyplot as plt

#Use Agg renderer
matplotlib.use('Agg')

width = 1024
height = 1024
dpi = 72.0

fig = plt.figure(1)
fig.set_dpi(dpi)
fig.set_figwidth(width/dpi)
fig.set_figheight(height/dpi)

plt.plot([1,2,3,4])
plt.ylabel('some numbers')

#draw the graph in memory
fig.canvas.draw()

texture = ue.create_transient_texture(width, height, EPixelFormat.PF_R8G8B8A8)
#copy pixels from matplotlib canvas to texture as RGBA
texture.texture_set_data(fig.canvas.buffer_rgba())

#save the texture
texture.save_package('/Game/FirstGraphTexture')

#open its editor
ue.open_editor_for_asset(texture)
