data_path = Path.home()/'data'/'mnist'

mnist_stats = ((0.15,), (0.15,))

epochs = 3

train_ds = MNIST(
    data_path, 
    train=True, 
    download=True,
    transform=T.Compose([
        T.RandomAffine(5, translate=(0.05, 0.05), scale=(0.9, 1.1)),
        T.ToTensor(),
        T.Normalize(*mnist_stats)
    ])
)

valid_ds = MNIST(
    data_path, 
    train=False, 
    transform=T.Compose([
        T.ToTensor(),
        T.Normalize(*mnist_stats)
    ])
)

phases = make_phases(train_ds, valid_ds, bs=1024, n_jobs=4)
model = Net()
opt = optim.Adam(model.parameters(), lr=1e-2)
cb = CallbacksGroup([
    RollingLoss(),
    Accuracy(),
    Scheduler(
        OneCycleSchedule(t=len(phases[0].loader) * epochs),
        mode='batch'
    ),
    StreamLogger()
    # early stopping
])

train(model, opt, phases, cb, epochs=epochs, loss_fn=F.cross_entropy)
