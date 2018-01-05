def main(args):
	from mpl_toolkits.mplot3d import Axes3D
	import matplotlib.pyplot as plt
	import sys
	import matplotlib.ticker as ticker

	arglist = sys.argv

	tpcLength = float(arglist[1])
	tpcHeight = float(arglist[2])
	tpcWidth = float(arglist[3])	

	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')

	x =[1,2,3,4,5,6,7,8,9,10]
	y =[5,6,2,3,13,4,1,2,4,8]
	z =[2,3,3,3,5,7,9,11,9,10]

	ax.scatter(x, y, z, c='r', marker='o')

	ax.set_xlabel('Z Label')
	ax.set_ylabel('X Label')
	ax.set_zlabel('Y Label')

	ax.patch.set_facecolor('white')

	ax.w_xaxis._axinfo.update({'grid' : {'color': 'white'}})	
	ax.w_yaxis._axinfo.update({'grid' : {'color': 'white'}})
	ax.w_zaxis._axinfo.update({'grid' : {'color': 'white'}})

	ax.w_xaxis.set_pane_color((1,1,1,1))
	ax.w_yaxis.set_pane_color((1,1,1,1))
	ax.w_zaxis.set_pane_color((1,1,1,1))

	axes = plt.gca()
	axes.set_ylim([0, tpcWidth])
	axes.set_zlim([-tpcHeight, tpcHeight])
	axes.set_xlim([0, tpcLength])

	plt.axis('off')

	plt.plot((0,0), (0,tpcWidth), (-tpcHeight,-tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,0), (0,0), (-tpcHeight,tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,0), (0,tpcWidth), (tpcHeight,tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,0), (tpcWidth,tpcWidth), (-tpcHeight,tpcHeight), 'b-', linewidth = 2)

	plt.plot((tpcLength,tpcLength), (0,tpcWidth), (-tpcHeight,-tpcHeight), 'b-', linewidth = 2)
        plt.plot((tpcLength, tpcLength), (0,0), (-tpcHeight,tpcHeight), 'b-', linewidth = 2)
        plt.plot((tpcLength,tpcLength), (0,tpcWidth), (tpcHeight,tpcHeight), 'b-', linewidth = 2)
        plt.plot((tpcLength,tpcLength), (tpcWidth,tpcWidth), (-tpcHeight,tpcHeight), 'b-', linewidth = 2)
 
        plt.plot((0,tpcLength), (0,0), (-tpcHeight,-tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,tpcLength), (0,0), (tpcHeight,tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,tpcLength), (tpcWidth,tpcWidth), (-tpcHeight,-tpcHeight), 'b-', linewidth = 2)
        plt.plot((0,tpcLength), (tpcWidth,tpcWidth), (tpcHeight,tpcHeight), 'b-', linewidth = 2)



#	plt.plot((0,tpcWidth), (-tpcHeight,-tpcHeight), (tpcLength,tpcLength), 'b-', linewidth = 2)
#       plt.plot((0,0), (-tpcHeight,tpcHeight), (tpcLength,tpcLength), 'b-', linewidth = 2)
#       plt.plot((0,tpcWidth), (tpcHeight,tpcHeight), (tpcLength,tpcLength), 'b-', linewidth = 2)
#       plt.plot((tpcWidth,tpcWidth), (-tpcHeight,tpcHeight), (tpcLength,tpcLength), 'b-', linewidth = 2)
#
#	plt.plot((0,0), (-tpcHeight,-tpcHeight), (0,tpcLength), 'b-', linewidth = 2)
#	plt.plot((0,0), (tpcHeight,tpcHeight), (0,tpcLength), 'b-', linewidth = 2)
#	plt.plot((tpcWidth,tpcWidth), (-tpcHeight,-tpcHeight), (0,tpcLength), 'b-', linewidth = 2)
#	plt.plot((tpcWidth,tpcWidth), (tpcHeight,tpcHeight), (0,tpcLength), 'b-', linewidth = 2)

	tick_spacing = 10
	ax.xaxis.set_major_locator(ticker.MultipleLocator(tick_spacing))
	ax.yaxis.set_major_locator(ticker.MultipleLocator(tick_spacing))
	ax.zaxis.set_major_locator(ticker.MultipleLocator(tick_spacing))

#	plt.show()
	
        return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
